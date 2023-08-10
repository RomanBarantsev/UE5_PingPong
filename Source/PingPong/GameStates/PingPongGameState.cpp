// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/Actors/PingPongBall.h"
#include "PingPong/GameModes/PingPongGameMode.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"


APingPongGameState::APingPongGameState()
{
	CurrentPlayersState=EPlayersStatus::NONE;
	bReplicates=true;
}

void APingPongGameState::BeginPlay()
{
	if(HasAuthority())
	{
		GameMode = Cast<APingPongGameMode>(GetDefaultGameMode());
		check(GameMode);
	}	
	Super::BeginPlay();
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
	DOREPLIFETIME( APingPongGameState, PlayerControllers );		
	DOREPLIFETIME( APingPongGameState, LoadedPlayers );	
}

void APingPongGameState::IncreaseLoadedPlayer_Implementation()
{
	LoadedPlayers++;
	if(LoadedPlayers==GameMode->GetPlayersCount())
	{
		for (auto PlayerController : PlayerControllers)
		{
			PlayerController->AllPlayersConnected();
		}
	}
}

void APingPongGameState::UpdateCountdown_Implementation()
{
	for (auto PlayerController : PlayerControllers)
	{
		PlayerController->AllPlayersReady(CountDown);		
	}
	if(CountDown==0)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongBall::StaticClass(),Actors);
		for (auto Actor : Actors)
		{
			if(Actor->GetOwner()==nullptr)
			{
				APingPongBall* PingPongBall = Cast<APingPongBall>(Actor);
				check(PingPongBall);
				PingPongBall->SetIsMoving(true);
			}
		}
	}
	if(CountDown<-1)
	{		
		GetWorld()->GetTimerManager().ClearTimer(CountDownHandle);		
	}
	CountDown--;
}

TArray<APingPongPlayerController*>& APingPongGameState::GetPlayersControllers()
{
	return PlayerControllers;
}

void APingPongGameState::IncreaseReadyPlayer_Implementation()
{
	ReadyPlayers++;	
	if(ReadyPlayers==GameMode->GetPlayersCount())
	{					
		GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &APingPongGameState::UpdateCountdown, 1.0f, true, 0.0f);
	}
}



