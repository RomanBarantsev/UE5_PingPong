// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void UOverlayWidget::NativeConstruct()
{
	//TODO Make text blocks for dynamic creating with Slate
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnReadyButtonPushed);
	PingPongPlayerController = Cast<APingPongPlayerController>(GetOwningPlayer());
	check(PingPongPlayerController);
	PingPongPlayerController->SetUIStatus(EUIStatus::UILoaded);
	TextScorePlayer1->SetVisibility(ESlateVisibility::Hidden);
	TextScorePlayer2->SetVisibility(ESlateVisibility::Hidden);
	TextNamePlayer1->SetVisibility(ESlateVisibility::Hidden);
	TextNamePlayer2->SetVisibility(ESlateVisibility::Hidden);
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
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1);
}

void UOverlayWidget::OnReadyButtonPushed()
{
	PingPongPlayerController->SetUIStatus(EUIStatus::ReadyButtonPressed);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
}

void UOverlayWidget::OnPlayersStateChanged(EPlayersStatus PlayersStatus)
{
	if(PlayersStatus==EPlayersStatus::AllPlayersConnected)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);
	}
	if(PlayersStatus==EPlayersStatus::AllPlayersIsReady)
	{
		TimerText->SetVisibility(ESlateVisibility::Visible);
	}
}


void UOverlayWidget::UpdateCountdown(int32 value)
{	
	if(value==0)
	{
		TimerText->SetText(FText::FromString("GO!"));		
	}
	if(value<=-1)
	{
		TimerText->SetVisibility(ESlateVisibility::Hidden);
	}
	if(value>0)
	{
		TimerText->SetText(FText::AsNumber(value));		
	}
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

