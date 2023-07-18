// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


APingPongGameState::APingPongGameState()
{
	bReplicates=true;
}

void APingPongGameState::BeginPlay()
{
	Super::BeginPlay();	
	//TODO Make it for multiple balls	
}

void APingPongGameState::AddScoreToPlayer1(int Value)
{
	ScorePlayer1+=Value;
	DelegateScore.Broadcast(ScorePlayer1,ScorePlayer2);
}

void APingPongGameState::AddScoreToPlayer2(int Value)
{
	ScorePlayer2+=Value;
	DelegateScore.Broadcast(ScorePlayer1,ScorePlayer2);
}

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongGameState, ScorePlayer1 );
	DOREPLIFETIME( APingPongGameState, ScorePlayer2 );
}



