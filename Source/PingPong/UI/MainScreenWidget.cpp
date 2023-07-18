// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PingPongGameMode.h"
#include "PingPong/PingPongGameState.h"
#include "PingPong/PingPongPlayerController.h"

void UMainScreenWidget::NativeConstruct()
{
	//TODO Make it for multiple balls
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UMainScreenWidget::SetReady);
	Super::NativeConstruct();	
}


void UMainScreenWidget::SetScore(int Player1, int Player2)
{
	TextScorePlayer1->SetText(FText::AsNumber(Player1));
	TextScorePlayer2->SetText(FText::AsNumber(Player2));
}

void UMainScreenWidget::SetReady()
{	
	APingPongPlayerController* PingPongPlayerController = Cast<APingPongPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if(PingPongPlayerController)
	{
		PingPongPlayerController->ToggleReadyState();
	}
}
