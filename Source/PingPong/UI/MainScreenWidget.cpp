// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PingPongGameMode.h"
#include "PingPong/PingPongGameState.h"
#include "PingPong/PingPongPlayerController.h"

void UMainScreenWidget::NativeConstruct()
{
	//TODO Make it for multiple balls
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
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

void UMainScreenWidget::OnReadyButtonPushed()
{
	
}

void UMainScreenWidget::OnPlayersStateChanged(EPlayersStatus PlayersStatus)
{
	ReadyButton->SetVisibility(ESlateVisibility::Visible);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);
}