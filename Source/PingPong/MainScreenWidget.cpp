// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenWidget.h"


void UMainScreenWidget::SetScoreText(int Value)
{
	ScoreText->SetText(FText::AsNumber(Value));
}

void UMainScreenWidget::SetTouchCountBar(float Value)
{
	TouchCountBar->SetPercent(Value);
}
