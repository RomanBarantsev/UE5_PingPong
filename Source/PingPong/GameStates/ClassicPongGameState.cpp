// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongGameState.h"

#include "GameFramework/GameMode.h"

AClassicPongGameState::AClassicPongGameState()
{
	ResetScore();
}

void AClassicPongGameState::AddMaxScore(int Score)
{
	ReadyPlayers=0;
	StartedPlayers=0;
	if (Score>=ScoreToEnd)
	{
		SetMatchState(MatchState::EnteringMap);
	}
	Super::AddMaxScore(Score);
}

void AClassicPongGameState::ResetScore()
{
	ScoreToEnd=15;
}

void AClassicPongGameState::IncreaseReadyPlayer()
{
	Super::IncreaseReadyPlayer();
	ResetScore();
}
