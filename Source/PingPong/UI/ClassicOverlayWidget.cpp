// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicOverlayWidget.h"

void UClassicOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//PlayersScoreTable->SetVisibility(ESlateVisibility::Collapsed);
	ShotButton->SetVisibility(ESlateVisibility::Collapsed);
	BallCost->SetVisibility(ESlateVisibility::Collapsed);
}
