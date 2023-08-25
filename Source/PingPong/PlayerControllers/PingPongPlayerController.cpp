// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include <Net/UnrealNetwork.h>

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Actors/PingPongPlatform.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/MainMenu.h"
#include "PingPong/UI/OverlayWidget.h"
#include "PingPong/UI/HUDs/PingPongHUD.h"

void APingPongPlayerController::BeginPlay()
{	
	if(IsLocalPlayerController())
	{
		AHUD* HUD = GetHUD();
		if(HUD)
		{
			PingPongHUD = Cast<APingPongHUD>(HUD);
		}
		check(PingPongHUD);		
	}	
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(PingPongGameState);		
	PingPongGameState->OnPlayersStateChanged.AddUObject(this,&ThisClass::OnPlayersStateChanged);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
	bShowMouseCursor=true;
	Super::BeginPlay();	
}

void APingPongPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Platform)
	{		
		Platform->Server_Rotate(0);
	}
}

void APingPongPlayerController::OnPlayersStateChanged_Implementation(EPlayersStatus PlayersStatus)
{
	
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



void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("LeftRight", this,&APingPongPlayerController::MoveRight);
	InputComponent->BindAxis("ForwardBackward", this,&APingPongPlayerController::MoveForward);
	InputComponent->BindAxis("MouseX", this,&APingPongPlayerController::RotatePlatform);
	InputComponent->BindAction("Menu",EInputEvent::IE_Pressed,this, &APingPongPlayerController::OpenMenu);
	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed,this, &APingPongPlayerController::Fire);
	InputComponent->BindAction("ScrollColor",EInputEvent::IE_Pressed,this, &APingPongPlayerController::ScrollColor);
}

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
		Platform->Server_MoveForward(AxisValue);
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

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if(Platform)
	{		
		Platform->Server_MoveRight(AxisValue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HAS NO PLATFORM!"));
	}
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

void APingPongPlayerController::OpenMenu()
{
	if(PingPongHUD->GetMainMenuWidget()->IsInViewport())
	{
		PingPongHUD->GetMainMenuWidget()->RemoveFromParent();	
		SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1);
	}
	else
	{
		PingPongHUD->GetMainMenuWidget()->AddToViewport(1);
		SetShowMouseCursor(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.2);
	}
}

void APingPongPlayerController::SetColorUI_Implementation(FLinearColor Color)
{
	PingPongHUD->GetOverlayWidget()->SetBallSquareColor(Color);
}

void APingPongPlayerController::ScrollColor_Implementation()
{
	PingPongPlayerState = GetPlayerState<APingPongPlayerState>();
	check(PingPongPlayerState);
	if (PingPongPlayerState->GetModificator()==EModificators::NONE)
	{
		PingPongPlayerState->SetModificator(EModificators::FAST);
	}
	else
	{
		int ModificatorNumber = static_cast<int>(PingPongPlayerState->GetModificator());
		ModificatorNumber++;
		PingPongPlayerState->SetModificator(static_cast<EModificators>(ModificatorNumber));
	}
	SetColorUI(PingPongGameState->GetModificatorColor(PingPongPlayerState->GetModificator()));
}

void APingPongPlayerController::SetNewScore_Implementation(int32 PlayerId, float Score)
{
	PingPongHUD->GetOverlayWidget()->UpdateScore(PlayerId,Score);
}

void APingPongPlayerController::SetScoreText_Implementation(int32 PlayerId)
{
	
	PingPongHUD->GetOverlayWidget()->SetPlayerScoreVisible(PlayerId);
}

void APingPongPlayerController::AllPlayersReady_Implementation(int32 CountDownValue)
{
	PingPongHUD->GetOverlayWidget()->OnPlayersStateChanged(EPlayersStatus::AllPlayersIsReady);
	PingPongHUD->GetOverlayWidget()->UpdateCountdown(CountDownValue);
}

void APingPongPlayerController::AllPlayersConnected_Implementation()
{
	PingPongHUD->GetOverlayWidget()->OnPlayersStateChanged(EPlayersStatus::AllPlayersConnected);
}

void APingPongPlayerController::SetUIStatus_Implementation(EUIStatus status)
{
	UIStatus=status;
	switch (UIStatus)
	{
	case 1: //UILoaded
		{
			PingPongGameState->IncreaseLoadedPlayer();
			break;
		}
	case 2: //ReadyButtonPressed
		{
			PingPongGameState->IncreaseReadyPlayer();
			break;
		}
	case 3: //UIPaused
		{
				
			break;
		}
	default:
		{
				
		}
	}
}
