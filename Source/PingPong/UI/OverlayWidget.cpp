// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void UOverlayWidget::NativeConstruct()
{
	//TODO Make text blocks for dynamic creating with Slate
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnReadyButtonPushed);
	PingPongPlayerController = Cast<APingPongPlayerController>(GetOwningPlayer());
	check(PingPongPlayerController);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	PingPongPlayerController->SetUIStatus(EUIStatus::UILoaded);
	TextScorePlayer1->SetVisibility(ESlateVisibility::Hidden);
	TextScorePlayer2->SetVisibility(ESlateVisibility::Hidden);
	TextNamePlayer1->SetVisibility(ESlateVisibility::Hidden);
	TextNamePlayer2->SetVisibility(ESlateVisibility::Hidden);
	GameOverText->SetVisibility(ESlateVisibility::Hidden);
	ShowWaitingForPlayers();
	Super::NativeConstruct();
}

void UOverlayWidget::SetScoreTextVisible()
{
	TextScorePlayer1->SetVisibility(ESlateVisibility::Visible);
	TextScorePlayer2->SetVisibility(ESlateVisibility::Visible);
	TextNamePlayer1->SetVisibility(ESlateVisibility::Visible);
	TextNamePlayer2->SetVisibility(ESlateVisibility::Visible);
}

void UOverlayWidget::ShowWaitingForPlayers()
{
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
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
	if(Result==TextScorePlayer1)
	{
		Result->SetText(FText::AsNumber(Score));
	}
	if(Result==TextScorePlayer2)
	{
		Result->SetText(FText::AsNumber(Score));
	}
	
}

void UOverlayWidget::SetPlayerScoreVisible(int32 PlayerId)
{
	SetScoreTextVisible();
	if(PlayersScoreMap.Num()==0)
	{
		PlayersScoreMap.FindOrAdd(PlayerId,TextScorePlayer1);
		TextNamePlayer1->SetText(FText::AsNumber(PlayerId));
	}
	else
	{
		PlayersScoreMap.FindOrAdd(PlayerId,TextScorePlayer2);
		TextNamePlayer2->SetText(FText::AsNumber(PlayerId));
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


