// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameStateBase.h"
#include "Net/UnrealNetwork.h"

APingPongGameStateBase::APingPongGameStateBase()
{
	bReplicates=true;
}

void APingPongGameStateBase::AddScoreToGreenPlayer(int Value)
{
	ScoreGreen+=Value;
}

void APingPongGameStateBase::AddScoreToBluePlayer(int Value)
{
	ScoreBlue+=Value;
}

void APingPongGameStateBase::AddValueToBallHits(int Value)
{
	BallHits+=Value;	
}

void APingPongGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongGameStateBase, ScoreGreen );
	DOREPLIFETIME( APingPongGameStateBase, ScoreBlue );
	DOREPLIFETIME( APingPongGameStateBase, BallHits );
}

