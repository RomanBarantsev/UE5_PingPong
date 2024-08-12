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
	Super::BeginPlay();
	if(BallModificatorsDataTable)
	{
		BallModificatorsRowNames =	BallModificatorsDataTable->GetRowNames();
	}
	else
	{
		UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Type::Quit,false);
		UE_LOG(LogTemp, Warning, TEXT("No Ball Modificators DataTable"));
	}
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

FLinearColor APingPongGameState::GetModificatorColor(EBallModificators modificator)
{		
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Color;
}

int32 APingPongGameState::GetModificatorPoints(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Points;
}

int32 APingPongGameState::GetShotCost(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->ShotCost;
}

int32 APingPongGameState::GetBallSpeed(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Speed;
}

float APingPongGameState::GetBallModificationValue(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->ModificatorValue;
}

FBallModificatorsTable* APingPongGameState::GetModificationRow(EBallModificators Modificator)
{
	for(const auto& RowName : BallModificatorsRowNames)
	{
		FBallModificatorsTable* RowMod = BallModificatorsDataTable->FindRow<FBallModificatorsTable>(RowName,"Context String",true);
		if(RowMod->BallModificators==Modificator)
		{
			return  RowMod;
		}			
	}
	return nullptr;
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



