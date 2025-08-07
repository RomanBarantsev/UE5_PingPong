// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongPlayerController.h"

#include "GameFramework/GameMode.h"
#include "PingPong/GameStates/ClassicPongGameState.h"
#include "PingPong/UI/OverlayWidget.h"

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

void AClassicPongPlayerController::HandleMatchStateChange(FName NewState)
{
	if (NewState == MatchState::WaitingToStart)
	{
		AClassicPongGameState* ClassicGS = Cast<AClassicPongGameState>(PingPongGameState);
		if (ClassicGS)
		{
			ClassicGS->ResetScore();
			if (PingPongHUD)
			{
				PingPongHUD->GetOverlayWidget()->AllPlayersReady();
			}			
		}
	}
	Super::HandleMatchStateChange(NewState);
}
