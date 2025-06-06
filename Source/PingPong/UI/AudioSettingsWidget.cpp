// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

void UAudioSettingsWidget::OnVolumeChanged(float X)
{
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		MasterSoundMix,
		MasterSound,
		X,
		1.0f,  // pitch
		0.0f   // fade-in time
	);
	UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
}

void UAudioSettingsWidget::NativeConstruct()
{
	check(MasterSound);
	check(MasterSoundMix);
	VolumeSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnVolumeChanged);
	Super::NativeConstruct();
}
