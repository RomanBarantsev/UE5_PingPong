// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidget.h"
#include "ClassicOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UClassicOverlayWidget : public UOverlayWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Line;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Score1;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Score2;
	UPROPERTY()
	int32 playerID;
	virtual void UpdateScore(int32 playerId, float Score) override;
};
