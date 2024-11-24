// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PingPongHUD.generated.h"

class APingPongGameState;
class UMainMenu;
class UOverlayWidget;
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
	TSubclassOf<UUserWidget> OverlayWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> MainMenuWidgetSubClass;
	UPROPERTY()
	UOverlayWidget* OverlayWidget;
	UPROPERTY()
	UMainMenu* MainMenuWidget;
	UPROPERTY()
	APingPongGameState* GameState;
public:
	UFUNCTION()
	UOverlayWidget* GetOverlayWidget();
	UFUNCTION()
	UMainMenu* GetMainMenuWidget();

private:
	UFUNCTION()
	void HandleMatchStateChange(FName NewState);
};
