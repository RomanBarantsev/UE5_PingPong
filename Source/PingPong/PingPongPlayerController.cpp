// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"
#include "PingPongGameState.h"
#include "Actors/PingPongPlatform.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenu.h"
#include "UI/OverlayWidget.h"
#include "UI/HUDs/PingPongHUD.h"

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
	Super::BeginPlay();	
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
	InputComponent->BindAction("Menu",EInputEvent::IE_Pressed,this, &APingPongPlayerController::OpenMenu);
}

void APingPongPlayerController::MoveRight(float AxisValue)
{	
	Server_PlatformMoveRight(AxisValue);
}

void APingPongPlayerController::MoveForward(float AxisValue)
{
	Server_PlatformMoveForward(AxisValue);
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

void APingPongPlayerController::AllPlayersReady_Implementation()
{
	PingPongHUD->GetOverlayWidget()->StartCountDown();
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
