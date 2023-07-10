// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include "Actors/PingPongPlatform.h"

void APingPongPlayerController::BeginPlay()
{
	AHUD* HUD = GetHUD();
	if(HUD)
	{
		PingPongHUD = Cast<APingPongHUD>(HUD);
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
	InputComponent->BindAxis("Move", this,&APingPongPlayerController::MoveRight);

}

void APingPongPlayerController::MoveRight(float AxisValue)
{
	if(AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning,TEXT("APingPongPlayerController::MoveRight"));
	}
	Server_PlatformMoveRight(AxisValue);
}

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if(Platform)
	{
		if(AxisValue != 0)
		{
			UE_LOG(LogTemp, Warning,TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation"));
		}
		Platform->Server_MoveRight(AxisValue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HASNO PLATFORM!"));
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
