// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicOverlayWidget.h"

#include "GameFramework/PlayerState.h"

void UClassicOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//PlayersScoreTable->SetVisibility(ESlateVisibility::Collapsed);
	ShotButton->SetVisibility(ESlateVisibility::Collapsed);
	BallCost->SetVisibility(ESlateVisibility::Collapsed);
	PlayersScoreTable->SetVisibility(ESlateVisibility::Collapsed);	
}

void UClassicOverlayWidget::UpdateScore(int32 playerId, float Score)
{	
	if (playerId == GetOwningPlayer()->PlayerState->GetPlayerId())
	{		
		Score2->SetText(FText::AsNumber(Score));
	}
	else
	{
		Score1->SetText(FText::AsNumber(Score));
	}
	//Super::UpdateScore(playerId, Score);
}
