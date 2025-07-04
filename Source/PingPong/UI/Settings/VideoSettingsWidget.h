﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VideoSettingsWidget.generated.h"


class USelectionBase;
class UCheckBox;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class PINGPONG_API UVideoSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
protected:
	UPROPERTY()
	TMap<FString, float> FrameRates{
		{ TEXT("30"),30.0f},	
		{ TEXT("60"),60.0f},
		{ TEXT("120"),120.0f},
		{ TEXT("240"),240.0f},
	};
	
	UFUNCTION()
	void OnResolutionChanged(FString InItemSelected, ESelectInfo::Type InSelectionType);
	
	void InitializeResolutionComboBox();
	UFUNCTION()
	void OnVSyncChanged(bool bIsChecked);
	void InitializeSync();
	UFUNCTION()
	void OnFrameRateChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	void InitializeFrameRate();
	UPROPERTY()
	TObjectPtr<UGameUserSettings> GameUserSettings;
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> VSyncCheckBox;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UComboBoxString> FrameRateComboBox;
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USelectionBase> ShadingQuality;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USelectionBase> GlobalIllumination;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USelectionBase> PostProcessing;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USelectionBase> VisualEffectQuality;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USelectionBase> ShadowQuality;

	UPROPERTY()
	TArray<FIntPoint> Resolutions;
};
