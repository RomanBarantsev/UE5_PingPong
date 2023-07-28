// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


APingPongGameState::APingPongGameState()
{
	CurrentPlayersState=EPlayersStatus::NONE;
	bReplicates=true;
}

void APingPongGameState::BeginPlay()
{
	Super::BeginPlay();	
	//TODO Make it for multiple balls	
}

void APingPongGameState::UpdateCharacterState(EPlayersStatus NewPlayersState)
{	
	CurrentPlayersState=NewPlayersState;
	if(OnPlayersStateChanged.IsBound())
		OnPlayersStateChanged.Broadcast(CurrentPlayersState);
}

EPlayersStatus APingPongGameState::GetPlayersStatus() const
{
	return CurrentPlayersState;
}

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME( APingPongGameState, CurrentPlayersState );
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	
}



