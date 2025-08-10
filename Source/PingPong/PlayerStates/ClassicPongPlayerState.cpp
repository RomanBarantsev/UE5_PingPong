// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongPlayerState.h"

#include "Kismet/GameplayStatics.h"

AClassicPongPlayerState::AClassicPongPlayerState()
{
	
}

void AClassicPongPlayerState::ChangeScore(int value)
{
	SetScore(value);
	//Super::ChangeScore(value);
}

void AClassicPongPlayerState::BeginPlay()
{
	ChangeScore(0);
	GameState = Cast<APongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	bReplicates=true;
	//Super::BeginPlay();
}
