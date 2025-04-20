// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseHUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API AGameHUD : public ABaseHUD
{
	GENERATED_BODY()
public:
	UFUNCTION()
	UOverlayWidget* GetOverlayWidget();
	UFUNCTION()
	UMainMenu* GetMainMenuWidget();
protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> OverlayWidgetSubClass;
private:
	UPROPERTY()
	UOverlayWidget* OverlayWidget;
	UPROPERTY()
	UMainMenu* MainMenuWidget;
	UPROPERTY()
	APingPongGameState* GameState;
	UFUNCTION()
	void CreateWidgets();
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void HandleMatchStateChange(FName NewState);
};
