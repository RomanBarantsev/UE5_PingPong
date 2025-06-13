// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingsWidget.generated.h"

class UPPSlider;
/**
 * 
 */
UCLASS()
class PINGPONG_API UAudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void VolumeChanged(const float X,USoundClass* sClass) const;
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere)
	USoundClass* MasterSound;
	UPROPERTY(EditAnywhere)
	USoundClass* EnvironmentSound;
	UPROPERTY(EditAnywhere)
	USoundClass* MusicSound;
	UPROPERTY(EditAnywhere)
	USoundMix* MasterSoundMix;
	UPROPERTY()
	TArray<UPPSlider*> Sliders;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> MasterSlider;	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> VoiceSlider;	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> MusicSlider;	
};
