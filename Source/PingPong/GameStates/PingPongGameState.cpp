// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/GameModes/PingPongGameMode.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/OverlayWidget.h"


APingPongGameState::APingPongGameState()
{
	bReplicates=true;	
}

void APingPongGameState::BeginPlay()
{	
	
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
	CalculateEnumBallModifications();
}

void APingPongGameState::OnrepPlayerStatesUpdated()
{
	if (!HasAuthority())
	{
		TArray<UUserWidget*> foundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(),foundWidgets,UOverlayWidget::StaticClass());
		for (auto FoundWidget : foundWidgets)
		{
			auto OverlayWidget = Cast<UOverlayWidget>(FoundWidget);
			if (OverlayWidget)
			{
				OverlayWidget->UpdatePlayerList();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("OnrepPlayerStatesUpdated"));
	}
	
}

void APingPongGameState::HandlePlayerStatesUpdated()
{
	PlayerStates=PlayerArray;
}

void APingPongGameState::AddMaxScore(int Score)
{
	if (Score>=ScoreToEnd)
	{
		SetMatchState(MatchState::WaitingPostMatch);
	}
}

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongGameState, ReadyPlayers );		
	DOREPLIFETIME( APingPongGameState, LoadedPlayers );	
	DOREPLIFETIME( APingPongGameState, StartedPlayers );	
	DOREPLIFETIME( APingPongGameState, PlayerControllers );
	DOREPLIFETIME( APingPongGameState, PlayerStates );
}

int32 APingPongGameState::GetCountDownTime()
{
	return CountDown;
}

void APingPongGameState::IncreaseLoadedPlayer_Implementation(APingPongPlayerController* PC)
{	
	if (PC)
		PlayerControllers.Add(PC);
	LoadedPlayers++;
	GameMode = Cast<APingPongGameMode>(GetDefaultGameMode());
	if(LoadedPlayers==GameMode->GetPlayersCount())
	{
		SetMatchState(MatchState::EnteringMap);
	}
	for (auto PlayerController : GetPlayersControllers())
	{		
		APingPongPlayerState* PlayerState = PlayerController->GetPlayerState<APingPongPlayerState>();
	}
}

void APingPongGameState::SetCountDownOnPlayerSide_Implementation()
{
	
}


void APingPongGameState::IncreaseStartedPlayers_Implementation()
{
	StartedPlayers++;
	if(StartedPlayers==GameMode->GetPlayersCount())
	{
		SetMatchState(MatchState::InProgress);
	}
}

void APingPongGameState::ServerPause_Implementation(bool state)
{
	if (state)
	{
		
		UGameplayStatics::SetGamePaused(GetWorld(),state);
	}		
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(),state);
	}
}

void APingPongGameState::DecreaseLoadedPlayer_Implementation(AController* PC)
{
	if (PC)
	{
		APingPongPlayerController* PCCast = Cast<APingPongPlayerController>(PC);
		PlayerControllers.Remove(PCCast);
	}		
	LoadedPlayers--;	
	for (auto PlayerController : GetPlayersControllers())
	{		
		APingPongPlayerState* PlayerState = PlayerController->GetPlayerState<APingPongPlayerState>();
	}
}

TArray<APingPongPlayerController*>& APingPongGameState::GetPlayersControllers()
{
	return PlayerControllers;
}

void APingPongGameState::CalculateEnumBallModifications()
{
	const UEnum* EnumPtr = StaticEnum<EBallModificators>();
	EBallModificatorsCount = EnumPtr ? EnumPtr->NumEnums() - 1 : 0;
	for (int i = 0; i < EBallModificatorsCount; ++i)
	{
		ModificationMap.Add(i,EBallModificators(i));
	}
}

int32 APingPongGameState::GeBallModificatorsCount()
{
	return EBallModificatorsCount;
}

EBallModificators APingPongGameState::GetModifcation(int8 mod)
{
	return ModificationMap[mod];
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

void APingPongGameState::SetMatchState_Implementation(FName NewState)
{
	MatchState = NewState;
	OnMatchStateChanged.Broadcast(NewState);	
}

FName APingPongGameState::GetMatchState() const
{
	return AGameState::GetMatchState();
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
		SetMatchState(MatchState::WaitingToStart);
	}
}



