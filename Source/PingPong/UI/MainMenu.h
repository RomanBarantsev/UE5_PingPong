// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	UButton* JoinGame;
	UPROPERTY(meta=(BindWidget))
	UButton* CreateGame;
	UPROPERTY(meta=(BindWidget))
	UButton* Settings;
	UPROPERTY(meta=(BindWidget))
	UBackgroundBlur* Blur;
	virtual void NativeConstruct() override;
};
