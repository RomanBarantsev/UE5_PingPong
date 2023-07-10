// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PingPongHUD.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION()
	void HUDMainWidowDraw();
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> WidgetTemplate;	
};
