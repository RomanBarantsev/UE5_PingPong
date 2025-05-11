// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInstance.generated.h"

class UMenuSlot;
class UButton;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class PINGPONG_API UMenuInstance : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuSlot> MenuItemClass;
	UPROPERTY(meta=(BindWidgetOptional))
	UVerticalBox* VerticalBox;
	UPROPERTY(meta=(BindWidgetOptional))
	UHorizontalBox*	HorizontalBox;
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* ApplyBtn;
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* ExitBtn;
	UFUNCTION()
	virtual void OnApplyBtnPushed();
	UFUNCTION()
	virtual void OnExitBtnPushed();
	virtual void NativeConstruct() override;
};
