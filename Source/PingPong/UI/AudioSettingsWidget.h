// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "AudioSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UAudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void OnVolumeChanged(float X);
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere)
	USoundClass* MasterSound;
	UPROPERTY(EditAnywhere)
	USoundMix* MasterSoundMix;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USlider> VolumeSlider;	
};
