// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/UI/MainMenu.h"
#include "PingPong/UI/OverlayWidget.h"


void APingPongHUD::BeginPlay()
{
	CreateWidgets();
	Super::BeginPlay();
	OverlayWidget->AddToViewport(0);
}

void APingPongHUD::CreateWidgets()
{
	OverlayWidget = Cast<UOverlayWidget>(CreateWidget(GetWorld(),OverlayWidgetSubClass));
	MainMenuWidget = Cast<UMainMenu>(CreateWidget(GetWorld(),MainMenuWidgetSubClass));
	GameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(this));
	if(GameState)
	{
		GameState->OnMatchStateChanged.AddDynamic(this,&APingPongHUD::HandleMatchStateChange);
	}
	check(OverlayWidget);
	check(MainMenuWidget);
}

UOverlayWidget* APingPongHUD::GetOverlayWidget()
{
	return OverlayWidget;
}

UMainMenu* APingPongHUD::GetMainMenuWidget()
{
	return MainMenuWidget;
}

void APingPongHUD::HandleMatchStateChange(FName NewState)
{
	if(NewState==MatchState::WaitingPostMatch)
	{
		OverlayWidget->ShowGameOverText();
	}
	if(NewState==MatchState::WaitingToStart)
	{		
		OverlayWidget->SetCountDownTime(GameState->GetCountDownTime());
		OverlayWidget->AllPlayersReady();		
	}
	if(NewState==MatchState::InProgress)
	{
		
	}
	if(NewState==MatchState::EnteringMap)
	{
		OverlayWidget->AllPlayersConnected();
	}
}


