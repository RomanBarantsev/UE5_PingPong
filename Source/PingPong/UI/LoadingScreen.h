// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	FNativeOnVisibilityChangedEvent OnVisibilityChangedEvent;
	void OnTimer();
	void VisibilityChanged(ESlateVisibility SlateVisibility);
	virtual void NativeConstruct() override;
	FTimerHandle RetryTimer;
};
