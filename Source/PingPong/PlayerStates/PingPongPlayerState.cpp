// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerState.h"
#include <Kismet/GameplayStatics.h>

#include "PingPong/GameStates/PingPongGameState.h"

void APingPongPlayerState::BeginPlay()
{
	GameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	#if WITH_EDITOR
	SetScore(50);
	#endif
	Super::BeginPlay();
	bReplicates=true;
	maxModificator = GameState->GeBallModificatorsCount()-1;
}

void APingPongPlayerState::NextModificator()
{
	if(Modificator==maxModificator)
	{
		Modificator=0;
		return;
	}
	Modificator++;
}

void APingPongPlayerState::PrevModificator()
{
	if(Modificator==0)
	{
		Modificator=maxModificator;
		return;
	}
	Modificator--;
}

EBallModificators APingPongPlayerState::GetModificator()
{
	return GameState->GetModifcation(Modificator);
}
