// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "Net/UnrealNetwork.h"

APingPongGameState::APingPongGameState()
{
	bReplicates=true;
}

void APingPongGameState::AddScoreToGreenPlayer(int Value)
{
	ScoreGreen+=Value;
}

void APingPongGameState::AddScoreToBluePlayer(int Value)
{
	ScoreBlue+=Value;
}

void APingPongGameState::AddValueToBallHits(int Value)
{
	BallHits+=Value;	
}

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongGameState, ScoreGreen );
	DOREPLIFETIME( APingPongGameState, ScoreBlue );
	DOREPLIFETIME( APingPongGameState, BallHits );
}

