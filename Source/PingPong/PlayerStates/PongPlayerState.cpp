// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerState.h"
#include <Kismet/GameplayStatics.h>

#include "PingPong/GameStates/PongGameState.h"

void APongPlayerState::BeginPlay()
{
	GameState = Cast<APongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	#if WITH_EDITOR
	SetScore(50);
	#endif
	Super::BeginPlay();
	bReplicates=true;
	maxModificator = GameState->GeBallModificatorsCount()-1;
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
