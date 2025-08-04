// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"

void ULoadingScreen::OnTimer()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoadingScreen::VisibilityChanged(ESlateVisibility SlateVisibility)
{
	GetWorld()->GetTimerManager().SetTimer(RetryTimer,this,&ThisClass::OnTimer,15.0f,false);
}

void ULoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();	
	OnNativeVisibilityChanged.Add(FNativeOnVisibilityChangedEvent::FDelegate::CreateUObject(this, &ThisClass::VisibilityChanged));
}
