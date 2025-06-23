// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Pong_GameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UPong_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config)
	FString PlayerName;	
};
