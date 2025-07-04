// Fill out your copyright notice in the Description page of Project Settings.


#include "Pong_GameUserSettings.h"
#include "Audio/AudioVolumeSettingsDataAsset.h"

UPong_GameUserSettings::UPong_GameUserSettings()
{
	if (!VolumeData) //TODO MAKE OWNING CLASS TO GET STATIC MEMBERS LIKE THIS DATA OR ANY STATES ETC FROM IT
	{
		VolumeData = Cast<UAudioVolumeSettingsDataAsset>(StaticLoadObject(
			UAudioVolumeSettingsDataAsset::StaticClass(),
			nullptr,
			TEXT("/Game/PingPong/Sounds/AudioDataAsset.AudioDataAsset")  // path to your asset
		));
	}
}

UPong_GameUserSettings::~UPong_GameUserSettings()
{
	if (VolumeData)
	{
		VolumeData->MasterVolume = MasterVolume;
		VolumeData->MusicVolume = MusicVolume;
		VolumeData->SFXVolume = SFXVolume;
	}
}

void UPong_GameUserSettings::LoadSettings(bool bForceReload)
{
	
	Super::LoadSettings(bForceReload);	
}

void UPong_GameUserSettings::SaveAudioSettings(USoundClass* SoundClass, float Value)
{
if (!SoundClass) return;
	if (SoundClass == VolumeData->MasterClass)
		MasterVolume = Value;
	else if (SoundClass == VolumeData->MusicClass)
		MusicVolume = Value;
	else if (SoundClass == VolumeData->SFXClass)
		SFXVolume = Value;
}

void UPong_GameUserSettings::LoadAuidoSettings()
{
	if (VolumeData)
	{
		VolumeData->MasterVolume = MasterVolume;
		VolumeData->MusicVolume = MusicVolume;
		VolumeData->SFXVolume = SFXVolume;
	}
}

