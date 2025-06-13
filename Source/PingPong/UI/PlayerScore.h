// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "PlayerScore.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UPlayerScore : public UUserWidget
{
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* PlayerName;
	UPROPERTY(Meta = (BindWidget))
	UHorizontalBox* HorizontalBox;
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	public:
	UFUNCTION()
	void SetPlayerName(FString Name);
	UFUNCTION()
	void SetPlayerScore(int Score);
};
