// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PingPongGameState.h"


void UMainScreenWidget::NativeConstruct()
{
	//TODO Make it for multiple balls
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if(PingPongGameState)
	{
		PingPongGameState->DelegateScore.AddUniqueDynamic(this,&UMainScreenWidget::SetScore);
	}
	Super::NativeConstruct();
}

void UMainScreenWidget::CountTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CountTimerHandle, this, &UMainScreenWidget::ChangeText, 1, true,0); 
}

void UMainScreenWidget::ChangeText()
{
	TimerText->SetText(FText::AsNumber(--count));
	if(count==0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountTimerHandle);
		TimerText->SetText(FText::FromString(""));
		//TODO TimerText->AddBinding?
	}
		
}

void UMainScreenWidget::SetScore(int Player1, int Player2)
{
	TextScorePlayer1->SetText(FText::AsNumber(Player1));
	TextScorePlayer2->SetText(FText::AsNumber(Player2));
}
