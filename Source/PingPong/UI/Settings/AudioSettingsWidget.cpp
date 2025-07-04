// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Settings/Pong_GameUserSettings.h"
#include "PingPong/Settings/Audio/AudioVolumeSettingsDataAsset.h"
#include "PingPong/UI/SlateClasses/PPSlider.h"
#include "Sound/SoundClass.h"

void UAudioSettingsWidget::VolumeChanged(const float X, USoundClass* sClass) const
{
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		VolumeData->SoundMix,
		sClass,
		X,
		1.0f,  // pitch
		0.0f   // fade-in time
	);	
	UGameplayStatics::PushSoundMixModifier(this, VolumeData->SoundMix);
	Pong_GameUserSettings->SaveAudioSettings(sClass, X);
}

void UAudioSettingsWidget::NativeConstruct()
{
	Pong_GameUserSettings = Cast<UPong_GameUserSettings>(GEngine->GetGameUserSettings());
	if (Pong_GameUserSettings)
	{
		Pong_GameUserSettings->LoadAuidoSettings();
	}
	check(Pong_GameUserSettings);
	if (!VolumeData)
	{
		VolumeData = Cast<UAudioVolumeSettingsDataAsset>(StaticLoadObject(
			UAudioVolumeSettingsDataAsset::StaticClass(),
			nullptr,
			TEXT("/Game/PingPong/Sounds/AudioDataAsset.AudioDataAsset")  // path to your asset
		));
	}
	check(VolumeData); 
	if (!VolumeData) return;
	MasterSlider->SetValue(VolumeData->MasterVolume);
	VoiceSlider->SetValue(VolumeData->SFXVolume);
	MusicSlider->SetValue(VolumeData->MusicVolume);
	VolumeChanged(VolumeData->MasterVolume,VolumeData->MasterClass);
	VolumeChanged(VolumeData->SFXVolume,VolumeData->SFXClass);
	VolumeChanged(VolumeData->MusicVolume,VolumeData->MusicClass);
	MasterSlider->AssociatedClass=VolumeData->MasterClass;
	VoiceSlider->AssociatedClass=VolumeData->SFXClass;
	MusicSlider->AssociatedClass=VolumeData->MusicClass;
	Sliders.Emplace(MasterSlider);
	Sliders.Emplace(VoiceSlider);
	Sliders.Emplace(MusicSlider);	
	for (const auto Slider : Sliders)
	{
		if (!Slider || !Slider->AssociatedClass) continue;
		auto cl  = Slider->AssociatedClass;
		Slider->OnCppMouseCaptureEnd.AddLambda([this](USoundClass* sClass, float Value)
		{
			VolumeChanged(Value,sClass);
		});
	}
	Super::NativeConstruct();
}
