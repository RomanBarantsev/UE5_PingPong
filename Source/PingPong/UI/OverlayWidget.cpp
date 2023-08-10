// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void UOverlayWidget::NativeConstruct()
{
	//TODO Make it for multiple balls
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnReadyButtonPushed);
	PingPongPlayerController = Cast<APingPongPlayerController>(GetOwningPlayer());
	check(PingPongPlayerController);
	PingPongPlayerController->SetUIStatus(EUIStatus::UILoaded);
	Super::NativeConstruct();
}

void UOverlayWidget::SetScoreText(int Player1, int Player2)
{
	TextScorePlayer1->SetText(FText::AsNumber(Player1));
	TextScorePlayer2->SetText(FText::AsNumber(Player2));
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
