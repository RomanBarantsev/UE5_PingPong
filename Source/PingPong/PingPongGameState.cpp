// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "PingPongGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


APingPongGameState::APingPongGameState()
{
	CurrentPlayersState=EPlayersStatus::NONE;
	ReadyPlayers=0;
	ScoreToEnd=100;
	bReplicates=true;
}

void APingPongGameState::BeginPlay()
{
	Super::BeginPlay();
	PingPongGameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	check(PingPongGameMode);
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
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongGameState, CurrentPlayersState );
	DOREPLIFETIME( APingPongGameState, ReadyPlayers );
		
}

void APingPongGameState::IncreaseReadyPlayer_Implementation()
{
	ReadyPlayers++;
	const APingPongGameMode* GameMode = Cast<APingPongGameMode>(GetDefaultGameMode());
	check(GameMode);
	if(ReadyPlayers==GameMode->GetPlayersCount())
	{
		CurrentPlayersState=EPlayersStatus::AllPlayersIsReady;
		TArray<APingPongPlayerController*> PlayerControllers = PingPongGameMode->GetPlayersControllers();
		
		// if(OnPlayersStateChanged.IsBound())
		// 	OnPlayersStateChanged.Broadcast(CurrentPlayersState);
	}
}



