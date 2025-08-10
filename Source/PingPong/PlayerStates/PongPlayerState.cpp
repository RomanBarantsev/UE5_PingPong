// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerState.h"
#include <Kismet/GameplayStatics.h>

#include "PingPong/GameStates/PongGameState.h"

void APongPlayerState::BeginPlay()
{
	GameState = Cast<APongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	Super::BeginPlay();
	bReplicates=true;
	maxModificator = GameState->GeBallModificatorsCount()-1;
	
#if WITH_EDITOR
	ChangeScore(50);
#endif
}

void APongPlayerState::NextModificator()
{
	if(Modificator==maxModificator)
	{
		Modificator=0;
		return;
	}
	Modificator++;
}

void APongPlayerState::PrevModificator()
{
	if(Modificator==0)
	{
		Modificator=maxModificator;
		return;
	}
	Modificator--;
}

EBallModificators APongPlayerState::GetModificator()
{
	return GameState->GetModifcation(Modificator);
}

void APongPlayerState::ChangeScore(int value)
{
	SetScore(value);
}
