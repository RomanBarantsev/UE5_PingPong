// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenu.h"
#include "MainScreenWidget.h"
#include "GameFramework/HUD.h"
#include "PingPongHUD.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void CreateWidgets();
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> MainScreenWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> MainMenuWidgetSubClass;
	UPROPERTY()
	UMainScreenWidget* MainScreenWidget;
	UPROPERTY()
	UMainMenu* MainMenuWidget;
public:
	UFUNCTION()
	UMainScreenWidget* GetMainScreenWidget();
	UFUNCTION()
	UMainMenu* GetMainMenuWidget();
};
