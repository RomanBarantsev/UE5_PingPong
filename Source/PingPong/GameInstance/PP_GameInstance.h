// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PP_GameInstance.generated.h"

class UPong_GameUserSettings;
class UPong_GameSettings;
/**
 * 
 */
UCLASS()
class PINGPONG_API UPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void Init() override;
	virtual void Shutdown() override;
	UPROPERTY()
	UPong_GameUserSettings* Pong_Settings;
	UPROPERTY()
	APlayerState* PlayerState;
	UPROPERTY()
	UGameUserSettings* Settings;
};
