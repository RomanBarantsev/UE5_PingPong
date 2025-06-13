// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Actors/PingPongPlatform.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/OverlayWidget.h"
#include "PingPong/UI/HUDs/BaseHUD.h"

void APingPongPlayerController::BeginPlay()
{
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (!PlatformName.Contains(TEXT("Android")))
	{
		SetVirtualJoystickVisibility(false);
	}
	if(IsLocalPlayerController())
	{
		AHUD* HUD = GetHUD();
		if(HUD)
		{
			PingPongHUD = Cast<AGameHUD>(HUD);
		}
		check(PingPongHUD);		
	}
	if(!PingPongPlayerState)
	{
		PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));	
	}
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(PingPongGameState);
	PingPongGameState->OnMatchStateChanged.AddDynamic(this,&ThisClass::HandleMatchStateChange);
	//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);	
	//DisableInput(this);
	ModificationsCount = PingPongGameState->GeBallModificatorsCount();	
	Super::BeginPlay();
	SetShowMouseCursor(false);
}

void APingPongPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Platform)
	{		
		Platform->Server_Rotate(0);
		if(!bIsMovingForward && !bIsMovingSides) Platform->Floating();
	}
	
}


APingPongPlayerController::APingPongPlayerController()
{
	bReplicates=true;
}

void APingPongPlayerController::SetStartTransform(FTransform NewStartTransform)
{
	StartTransform=NewStartTransform;
}

void APingPongPlayerController::PreInitializeComponents()
{

	Super::PreInitializeComponents();
	bReplicates=true;
}

void APingPongPlayerController::SpawnPlatform_Implementation()
{
	Platform = (APingPongPlatform*)GetWorld()->SpawnActor<APingPongPlatform>(PlatformClass);
    if(Platform)
    {
		Platform->SetActorLocation(StartTransform.GetLocation());
		Platform->SetActorRotation(StartTransform.GetRotation());
    	Platform->SetOwner(GetPawn());
    }

}

bool APingPongPlayerController::SpawnPlatform_Validate()
{
	return PlatformClass != NULL;
}
#if PLATFORM_ANDROID
void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("LeftRight", this,&APingPongPlayerController::MoveRight);
	InputComponent->BindAxis("ForwardBackward", this,&APingPongPlayerController::MoveForward);
	InputComponent->BindAction("Menu",EInputEvent::IE_Pressed,this, &APingPongPlayerController::OpenMenu);
	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed,this, &APingPongPlayerController::Fire);
	//TODO need to fix it - due to compilation error  G:/Git/UE5_PingPong/Source/PingPong/PlayerControllers/PingPongPlayerController.cpp(101,18): error: no matching member function for call to 'BindAction'
	//InputComponent->BindAction("ScrollColor",EInputEvent::IE_Pressed,this, &APingPongPlayerController::ScrollColor);
	InputComponent->BindAxis("AndroidRotate", this,&APingPongPlayerController::RotatePlatform);	
	InputComponent->bBlockInput = true;	
}

#else
	
void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("LeftRight", this,&APingPongPlayerController::MoveRight);
	InputComponent->BindAxis("ForwardBackward", this,&APingPongPlayerController::MoveForward);
	InputComponent->BindAxis("ScrollColor",this, &APingPongPlayerController::ScrollColor);
	InputComponent->BindAxis("MouseX", this,&APingPongPlayerController::RotatePlatform);InputComponent->BindAction("Menu",EInputEvent::IE_Pressed,this, &APingPongPlayerController::OpenMenu);
	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed,this, &APingPongPlayerController::Fire);
	
}
#endif


void APingPongPlayerController::MoveRight(float AxisValue)
{	
	Server_PlatformMoveRight(AxisValue);
}

void APingPongPlayerController::MoveForward(float AxisValue)
{
	Server_PlatformMoveForward(AxisValue);
}

void APingPongPlayerController::RotatePlatform(float AxisValue)
{
	Server_PlatformRotate(AxisValue);
}

void APingPongPlayerController::RequstPause_Implementation(bool state)
{
	PingPongGameState->ServerPause(state);
}

bool APingPongPlayerController::RequstPause_Validate(bool state)
{
	return true;
}

void APingPongPlayerController::OpenMenu_Implementation()
{
	if (bShowMouseCursor)
	{
		SetShowMouseCursor(false);
		PingPongHUD->SwitchUI(Widgets::MainMenu);		
		RequstPause(false);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		SetShowMouseCursor(true);
		PingPongHUD->SwitchUI(Widgets::MainMenu);
		RequstPause(true);
	}
}

bool APingPongPlayerController::OpenMenu_Validate()
{	
	return true;
}

void APingPongPlayerController::Fire_Implementation()
{
	if(Platform)
	{
		PingPongPlayerState = GetPlayerState<APingPongPlayerState>();
		check(PingPongPlayerState);
		Platform->Server_Fire(PingPongPlayerState->GetModificator());
	}
}

bool APingPongPlayerController::Fire_Validate()
{
	return true;
}

void APingPongPlayerController::Server_PlatformMoveForward_Implementation(float AxisValue)
{
	if(Platform)
	{		
		Platform->Server_GetForwardValue(AxisValue);
		if(AxisValue==0) bIsMovingForward=false;
		else bIsMovingForward=true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HAS NO PLATFORM!"));
	}
}

bool APingPongPlayerController::Server_PlatformMoveForward_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if(Platform)
	{		
		Platform->Server_GetRightValue(AxisValue);
		if(AxisValue==0) bIsMovingSides=false;
		else bIsMovingSides=true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HAS NO PLATFORM!"));
	}
}

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}



bool APingPongPlayerController::Initialize_Validate()
{
	return true;
}

void APingPongPlayerController::Initialize_Implementation()
{
	if(Platform)
		Platform->Destroy();
	SpawnPlatform();
}

void APingPongPlayerController::Server_PlatformRotate_Implementation(float AxisValue)
{
	if(Platform)
	{		
		Platform->Server_Rotate(AxisValue);
	}
}

bool APingPongPlayerController::Server_PlatformRotate_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::ScrollColorOnServer_Implementation(float Axis)
{
	PingPongPlayerState = GetPlayerState<APingPongPlayerState>();
	if (Axis > 0)
	{				
		PingPongPlayerState->NextModificator();
	}
	else
	{
		PingPongPlayerState->PrevModificator();
	}
	// Update UI
	SetColorAndPriceUI(
		PingPongGameState->GetModificatorColor(PingPongPlayerState->GetModificator()),
		PingPongGameState->GetShotCost(PingPongPlayerState->GetModificator())
	);
}

bool APingPongPlayerController::ScrollColorOnServer_Validate(float Axis)
{
	return true;
}

void APingPongPlayerController::ScrollColor(float Axis)
{
	
	if (FMath::IsNearlyZero(Axis)) // Handle floating-point precision
		return;
	ScrollColorOnServer(Axis);
	
}

void APingPongPlayerController::HandleMatchStateChange(FName NewState)
{
	if(NewState==MatchState::WaitingPostMatch)
	{
		DisableInput(this);
	}
	if(NewState==MatchState::InProgress)
	{
		EnableInput(this);
	}
}

void APingPongPlayerController::SetColorAndPriceUI_Implementation(FLinearColor Color,int32 Price)
{
	PingPongHUD->GetOverlayWidget()->SetBallSquareColor(Color);
	PingPongHUD->GetOverlayWidget()->SetBallShotCostText(Price);
}

void APingPongPlayerController::SetNewScore_Implementation(int32 PlayerId, float Score)
{
	PingPongHUD->GetOverlayWidget()->UpdateScore(PlayerId,Score);
}

void APingPongPlayerController::SetScoreText_Implementation(int32 PlayerId,const FString& playerName)
{
	
	PingPongHUD->GetOverlayWidget()->SetPlayerScoreVisible(PlayerId,playerName);
}

void APingPongPlayerController::SetUIStatus_Implementation(EUIStatus status)
{
	if(HasAuthority() && UKismetSystemLibrary::IsDedicatedServer(GetWorld())) return; 
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	UIStatus=status;
	switch (UIStatus)
	{
	case EUIStatus::UILoaded: //UILoaded
		{
			PingPongGameState->IncreaseLoadedPlayer();
			break;
		}
	case EUIStatus::ReadyButtonPressed: //ReadyButtonPressed
		{
			PingPongGameState->IncreaseReadyPlayer();
			break;
		}
	case EUIStatus::UIPaused: //UIPaused
		{
				
			break;
		}
	case EUIStatus::Started:
		{
			PingPongGameState->IncreaseStartedPlayers();
			break;
		}
	default:
		{
				
		}
	}
}
