// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/UI/SlateClasses/PPSlider.h"
#include "Sound/SoundClass.h"

void UAudioSettingsWidget::VolumeChanged(const float X, USoundClass* sClass) const
{
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		MasterSoundMix,
		sClass,
		X,
		1.0f,  // pitch
		0.0f   // fade-in time
	);	
	UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
	
}

void UAudioSettingsWidget::NativeConstruct()
{
	MasterSlider->AssociatedClass=MasterSound;
	VoiceSlider->AssociatedClass=EnvironmentSound;
	MusicSlider->AssociatedClass=MusicSound;
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
