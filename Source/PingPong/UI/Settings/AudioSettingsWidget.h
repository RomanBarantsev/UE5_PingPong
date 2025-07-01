// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AudioSettingsWidget.generated.h"

class UPong_GameUserSettings;
class UAudioVolumeSettingsDataAsset;
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
	UPROPERTY()
	UPong_GameUserSettings* Pong_GameUserSettings;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	UAudioVolumeSettingsDataAsset* VolumeData;
	UPROPERTY()
	TArray<UPPSlider*> Sliders;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> MasterSlider;	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> VoiceSlider;	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UPPSlider> MusicSlider;	
};
