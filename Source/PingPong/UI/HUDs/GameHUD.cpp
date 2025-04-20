// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/UI/MainMenu.h"
#include "PingPong/UI/OverlayWidget.h"

UOverlayWidget* AGameHUD::GetOverlayWidget()
{
	return OverlayWidget;
}

UMainMenu* AGameHUD::GetMainMenuWidget()
{
	return MainMenuWidget;
}

void AGameHUD::CreateWidgets()
{
	if (OverlayWidgetSubClass)
		OverlayWidget = Cast<UOverlayWidget>(CreateWidget(GetWorld(),OverlayWidgetSubClass));
	if (MainMenuWidgetSubClass)
		MainMenuWidget = Cast<UMainMenu>(CreateWidget(GetWorld(),MainMenuWidgetSubClass));		
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if(!GameState)
	{		
		GameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(this));
		if (GameState)
			GameState->OnMatchStateChanged.AddDynamic(this,&AGameHUD::HandleMatchStateChange);
	}
	if (OverlayWidgetSubClass)
	{
		UUserWidget* Overlay = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetSubClass);
		UIWidgetsMap.Add(Widgets::Overlay,Overlay);
	}
	CreateWidgets();
}


void AGameHUD::HandleMatchStateChange(FName NewState)
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
