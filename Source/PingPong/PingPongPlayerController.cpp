// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include "PingPongGameMode.h"
#include "Actors/PingPongPlatform.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void APingPongPlayerController::BeginPlay()
{
	if(!GIsServer)
	{
		AHUD* HUD = GetHUD();
		if(HUD)
		{
			PingPongHUD = Cast<APingPongHUD>(HUD);
		}
		
		MainMenu = CreateWidget<UMainMenu>(this,MainMenuClass);	
	}
	Super::BeginPlay();	
}

APingPongPlayerController::APingPongPlayerController()
{
	
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
	if(MainMenu->IsInViewport())
	{
		MainMenu->RemoveFromParent();	
		SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1);
	}
	else
	{
		MainMenu->AddToViewport(1);
		SetShowMouseCursor(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.2);
	}
}

void APingPongPlayerController::ToggleReadyState_Implementation()
{
	bIsReady=!bIsReady;
	APingPongGameMode* PingPongGameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(PingPongGameMode)
	{
		PingPongGameMode->PlayerReady();
	}
}

