// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongPlayerController.h"

void AClassicPongPlayerController::RotatePlatform(float AxisValue)
{
	//Super::RotatePlatform(AxisValue);
}

void AClassicPongPlayerController::MoveForward(float AxisValue)
{
	//Super::MoveForward(AxisValue);
}

void AClassicPongPlayerController::MoveRight(float AxisValue)
{
	AxisValue=-AxisValue;
	Super::MoveRight(AxisValue);
}

void AClassicPongPlayerController::Fire()
{
	//Super::Fire();
}

void AClassicPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MouseY", this,&APongPlayerController::MoveRight);	
	
}
