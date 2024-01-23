// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerState.h"
#include <Kismet/GameplayStatics.h>

#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/Pawns/PingPongPlayerPawn.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void APingPongPlayerState::BeginPlay()
{
	GameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	#if WITH_EDITOR
	SetScore(50);
	#endif
	Super::BeginPlay();
}

EBallModificators APingPongPlayerState::GetModificator()
{
	return Modificator;
}

void APingPongPlayerState::SetModificator(EBallModificators modificator)
{
	Modificator = modificator;
}
