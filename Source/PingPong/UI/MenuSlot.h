// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSlot.generated.h"

class UTextBlock;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class PINGPONG_API UMenuSlot : public UUserWidget
{
	UPROPERTY()
	UHorizontalBox* HorizontalBox;
	UPROPERTY()
	UTextBlock* TextBlock;
	
	virtual void NativeConstruct() override;
	GENERATED_BODY()
	
};
