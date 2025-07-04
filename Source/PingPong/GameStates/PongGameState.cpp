// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/GameModes/PongGameMode.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"
#include "PingPong/PlayerStates/PongPlayerState.h"
#include "PingPong/UI/OverlayWidget.h"


APongGameState::APongGameState()
{
	bReplicates=true;	
}

void APongGameState::BeginPlay()
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

void APongGameState::OnrepPlayerStatesUpdated()
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

void APongGameState::HandlePlayerStatesUpdated()
{
	PlayerStates=PlayerArray;
}

void APongGameState::AddMaxScore(int Score)
{
	if (Score>=ScoreToEnd)
	{
		SetMatchState(MatchState::WaitingPostMatch);
	}
}

void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APongGameState, ReadyPlayers );		
	DOREPLIFETIME( APongGameState, LoadedPlayers );	
	DOREPLIFETIME( APongGameState, StartedPlayers );	
	DOREPLIFETIME( APongGameState, PlayerStates );
}

int32 APongGameState::GetCountDownTime()
{
	return CountDown;
}

void APongGameState::IncreaseReadyPlayer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("READY"));
	ReadyPlayers++;
	GameMode = Cast<APongGameMode>(GetDefaultGameMode());
	if(ReadyPlayers==GameMode->GetPlayersCount())
	{
		SetMatchState(MatchState::WaitingToStart);
	}
}

void APongGameState::IncreaseLoadedPlayer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("INCREASE"));
	LoadedPlayers++;
	GameMode = Cast<APongGameMode>(GetDefaultGameMode());
	if(PlayerStates.Num()==GameMode->GetPlayersCount())
	{		
		SetMatchState(MatchState::EnteringMap);
	}
}

void APongGameState::SetCountDownOnPlayerSide_Implementation()
{
	
}


void APongGameState::IncreaseStartedPlayers_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("STARTED++"));
	StartedPlayers++;
	GameMode = Cast<APongGameMode>(GetDefaultGameMode());
	if(StartedPlayers==GameMode->GetPlayersCount())
	{
		SetMatchState(MatchState::InProgress);
	}
}

void APongGameState::ServerPause_Implementation(bool state)
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

void APongGameState::DecreaseLoadedPlayer_Implementation(AController* PC)
{
	//TODO make reconnect possible.
	SetMatchState(MatchState::WaitingPostMatch);
}

void APongGameState::UpdatePlayersScore_Implementation(int32 playerId, int32 Score)
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
				OverlayWidget->UpdateScore(playerId, Score);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("OnrepPlayerStatesUpdated"));
	}
}

void APongGameState::CalculateEnumBallModifications()
{
	const UEnum* EnumPtr = StaticEnum<EBallModificators>();
	EBallModificatorsCount = EnumPtr ? EnumPtr->NumEnums() - 1 : 0;
	for (int i = 0; i < EBallModificatorsCount; ++i)
	{
		ModificationMap.Add(i,EBallModificators(i));
	}
}

int32 APongGameState::GeBallModificatorsCount()
{
	return EBallModificatorsCount;
}

EBallModificators APongGameState::GetModifcation(int8 mod)
{
	return ModificationMap[mod];
}

FLinearColor APongGameState::GetModificatorColor(EBallModificators modificator)
{		
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Color;
}

int32 APongGameState::GetModificatorPoints(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Points;
}

int32 APongGameState::GetShotCost(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->ShotCost;
}

int32 APongGameState::GetBallSpeed(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->Speed;
}

float APongGameState::GetBallModificationValue(EBallModificators modificator)
{
	FBallModificatorsTable* RowMod = GetModificationRow(modificator);
	return RowMod->ModificatorValue;
}

FBallModificatorsTable* APongGameState::GetModificationRow(EBallModificators Modificator)
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

void APongGameState::SetMatchState_Implementation(FName NewState)
{
	UE_LOG(LogTemp, Display, TEXT("SetMatch	State_Implementation"));
	MatchState = NewState;
	OnMatchStateChanged.Broadcast(NewState);	
}

FName APongGameState::GetMatchState() const
{
	return AGameState::GetMatchState();
}

