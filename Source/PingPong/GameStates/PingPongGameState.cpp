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
	const FBallsStruct Fast = {FLinearColor::Green,2,2,1200};
	const FBallsStruct Slow = {FLinearColor::Red,3,2,1200};
	const FBallsStruct Shrink = {FLinearColor::Yellow,4,3,1400};
	const FBallsStruct Expand = {FLinearColor::Blue,5,3,1400};
	const FBallsStruct ReverseControl = {FLinearColor::White,6,4,1600};
	const FBallsStruct LightsOff = {FLinearColor::Black,7,5,1600};
	const FBallsStruct None = {FLinearColor::Gray,8,1,1000};
	ModificatorColors.FindOrAdd(EModificators::Fast,Fast);
	ModificatorColors.FindOrAdd(EModificators::Slow,Slow);
	ModificatorColors.FindOrAdd(EModificators::Shrink,Shrink);
	ModificatorColors.FindOrAdd(EModificators::Expand,Expand);
	ModificatorColors.FindOrAdd(EModificators::ReverseControl,ReverseControl);
	ModificatorColors.FindOrAdd(EModificators::LightsOff,LightsOff);
	ModificatorColors.FindOrAdd(EModificators::None,None);
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
	GameMode = Cast<APingPongGameMode>(GetDefaultGameMode());
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
	FBallsStruct Struct = ModificatorColors.FindRef(modificator);	
	return Struct.Color;
}

int32 APingPongGameState::GetModificatorPoints(EModificators modificator)
{
	FBallsStruct Struct = ModificatorColors.FindRef(modificator);	
	return Struct.Points;
}

int32 APingPongGameState::GetModificatorShotCost(EModificators modificator) const
{
	FBallsStruct Struct = ModificatorColors.FindRef(modificator);	
	return Struct.ShotCost;
}

float APingPongGameState::GetModificatorSpeed(EModificators modificator) const
{
	FBallsStruct Struct = ModificatorColors.FindRef(modificator);
	return Struct.Speed;
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



