// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PingPongGameState.h"
#include "PingPong/PingPongPlayerController.h"

void UMainScreenWidget::NativeConstruct()
{
	//TODO Make it for multiple balls
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UMainScreenWidget::OnReadyButtonPushed);
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(PingPongGameState);	
	PingPongGameState->OnPlayersStateChanged.AddUObject(this,&ThisClass::OnPlayersStateChanged);	
	if(PingPongGameState->GetPlayersStatus()==EPlayersStatus::AllPlayersConnected)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);
	}
	Super::NativeConstruct();
}

void UMainScreenWidget::SetScoreText(int Player1, int Player2)
{
	TextScorePlayer1->SetText(FText::AsNumber(Player1));
	TextScorePlayer2->SetText(FText::AsNumber(Player2));
}

void UMainScreenWidget::ShowWaitingForPlayers()
{
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(),0));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1);
}

void UMainScreenWidget::OnReadyButtonPushed()
{
	PingPongGameState->IncreaseReadyPlayer();
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMainScreenWidget::OnPlayersStateChanged(EPlayersStatus PlayersStatus)
{
	if(PlayersStatus==EPlayersStatus::AllPlayersConnected)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);
	}
	if(PlayersStatus==EPlayersStatus::AllPlayersIsReady)
	{
		StartCountDown();
	}
}

void UMainScreenWidget::StartCountDown()
{
	GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &UMainScreenWidget::UpdateCountdown, 1.0f, true, 0.0f);
	TimerText->SetVisibility(ESlateVisibility::Visible);
}

void UMainScreenWidget::UpdateCountdown()
{
	if(CountDownSeconds==0)
	{
		TimerText->SetText(FText::FromString("GO!"));		
	}
	if(CountDownSeconds<=-1)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountTimerHandle);
		TimerText->SetVisibility(ESlateVisibility::Hidden);
	}
	if(CountDownSeconds>0)
	{
		TimerText->SetText(FText::AsNumber(CountDownSeconds));
		
	}
	CountDownSeconds--;	
}
