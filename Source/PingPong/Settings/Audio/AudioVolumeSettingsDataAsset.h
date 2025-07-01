// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AudioVolumeSettingsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UAudioVolumeSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Volume")
	USoundClass* MasterClass;

	UPROPERTY(EditAnywhere, Category = "Volume")
	USoundClass* MusicClass;

	UPROPERTY(EditAnywhere, Category = "Volume")
	USoundClass* SFXClass;

	UPROPERTY(EditAnywhere, Category = "Volume")
	USoundMix* SoundMix;

	UPROPERTY(EditAnywhere, Category = "Volume")
	float MasterVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Volume")
	float MusicVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Volume")
	float SFXVolume = 1.0f;
private:
	virtual void PostLoad() override;
	virtual void BeginDestroy() override;
};
