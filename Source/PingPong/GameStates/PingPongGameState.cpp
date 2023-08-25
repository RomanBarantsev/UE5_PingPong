// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/Actors/PingPongBall.h"
#include "PingPong/GameModes/PingPongGameMode.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"


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
	ModificatorColors.FindOrAdd(EModificators::FAST,FLinearColor::Green);
	ModificatorColors.FindOrAdd(EModificators::SLOW,FLinearColor::Red);
	ModificatorColors.FindOrAdd(EModificators::SHRINK,FLinearColor::Yellow);
	ModificatorColors.FindOrAdd(EModificators::EXPAND,FLinearColor::Blue);
	ModificatorColors.FindOrAdd(EModificators::REVERSE_CONTROL,FLinearColor::White);
	ModificatorColors.FindOrAdd(EModificators::LIGHTS_OFF,FLinearColor::Black);
	ModificatorColors.FindOrAdd(EModificators::NONE,FLinearColor::Gray);
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
		TArray<AActor*> findActors;		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongBall::StaticClass(),findActors);
		for (auto findActor : findActors)
		{
			if(findActor->GetOwner()==nullptr)
			{
				APingPongBall* PingPongBall = Cast<APingPongBall>(findActor);
				check(PingPongBall);
				PingPongBall->StartMove();
				PingPongBall->RotateBallTo();
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

FLinearColor APingPongGameState::GetModificatorColor(EModificators modificator)
{
	FLinearColor Color = ModificatorColors.FindRef(modificator);
	return Color;
}



void APingPongGameState::UpdatePlayersScore_Implementation(int32 playerId, int32 Score)
{
	for (auto PlayerController : PlayerControllers)
	{
		PlayerController->SetNewScore(playerId,Score);
	}
}

void APingPongGameState::IncreaseReadyPlayer_Implementation()
{
	ReadyPlayers++;	
	if(ReadyPlayers==GameMode->GetPlayersCount())
	{
		for (auto PlayerController : PlayerControllers)
		{
			for (auto PlayerControllerState : PlayerControllers)
			{
				APingPongPlayerState* PlayerState = PlayerControllerState->GetPlayerState<APingPongPlayerState>();
				PlayerController->SetScoreText(PlayerState->GetPlayerId());	
			}
			
		}		
		GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &APingPongGameState::UpdateCountdown, 1.0f, true, 0.0f);
	}
}



