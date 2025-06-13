// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidget.h"

#include "PlayerScore.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"

void UOverlayWidget::NativeConstruct()
{
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (!PlatformName.Contains(TEXT("Android")))
	{
		MenuBtn->SetVisibility(ESlateVisibility::Hidden);
	}	
	//TODO Make text blocks for dynamic creating with Slate
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnReadyButtonPushed);
	MenuBtn->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnMenuButtonPushed);
	PingPongPlayerController = Cast<APingPongPlayerController>(GetOwningPlayer());
	check(PingPongPlayerController);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	PingPongPlayerController->SetUIStatus(EUIStatus::UILoaded);
	GameOverText->SetVisibility(ESlateVisibility::Hidden);
	ShowWaitingForPlayers();
	Super::NativeConstruct();
}

void UOverlayWidget::ShowWaitingForPlayers()
{
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Visible);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(),0));
}

void UOverlayWidget::SetCountDownTime(int32 Time)
{
	CountdownTime = Time;
}

void UOverlayWidget::OnReadyButtonPushed()
{
	PingPongPlayerController->SetUIStatus(EUIStatus::ReadyButtonPressed);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
}

void UOverlayWidget::OnMenuButtonPushed()
{
	PingPongPlayerController->OpenMenu();
}

void UOverlayWidget::UpdateCountdown()
{
	if(CountdownTime==0)
	{
		TimerText->SetText(FText::FromString("GO!"));		
	}
	if(CountdownTime<=-1)
	{
		TimerText->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		PingPongPlayerController->SetUIStatus(EUIStatus::Started);
	}
	if(CountdownTime>0)
	{
		TimerText->SetText(FText::AsNumber(CountdownTime));		
	}
	--CountdownTime;
}

void UOverlayWidget::UpdateScore(int32 playerId, float Score)
{
	auto Result = PlayersScoreMap.FindRef(playerId);
		
}

void UOverlayWidget::SetPlayerScoreVisible(int32 PlayerId,FString playerName)
{
	UUserWidget* score = CreateWidget<UUserWidget>(this,PlayerScoreWidget);
	PlayersScoreTable->AddChild(score);
	UPlayerScore* scoreWidget = Cast<UPlayerScore>(score);
	PlayersScoreMap.Add(PlayerId,scoreWidget);
	if (scoreWidget)
	{
		scoreWidget->SetPlayerName(playerName);
		scoreWidget->SetPlayerScore(0);
	}
}

void UOverlayWidget::SetBallSquareColor(FLinearColor Color)
{
	BallColor->SetBrushColor(Color);
}

void UOverlayWidget::SetBallShotCostText(int32 score)
{
	BallCost->SetText(FText::AsNumber(score));
}

void UOverlayWidget::ShowGameOverText()
{
	GameOverText->SetVisibility(ESlateVisibility::Visible);
	
}

void UOverlayWidget::AllPlayersConnected()
{
	ReadyButton->SetVisibility(ESlateVisibility::Visible);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);
}

void UOverlayWidget::AllPlayersReady()
{	
	TimerText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UOverlayWidget::UpdateCountdown, 1.0f, true);	
}


