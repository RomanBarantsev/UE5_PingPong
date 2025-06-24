// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Pong_GameUserSettings.generated.h"

class UAudioVolumeSettingsDataAsset;
/**
 * 
 */
UCLASS()
class PINGPONG_API UPong_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	UPong_GameUserSettings();
	~UPong_GameUserSettings();
public:
	UPROPERTY(Config)
	FString PlayerName;
	UPROPERTY()
	UAudioVolumeSettingsDataAsset* VolumeData;
	UPROPERTY(Config)
	float MasterVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float SFXVolume;
	virtual void LoadSettings(bool bForceReload = false) override;
	void SaveAudioSettings(USoundClass* SoundClass, float Value);
	void LoadAuidoSettings();
	
};
