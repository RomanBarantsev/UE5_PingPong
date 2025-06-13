// Fill out your copyright notice in the Description page of Project Settings.


#include "VideoSettingsWidget.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PingPong/UI/SelectionBase.h"

namespace
{	
	typedef int32 (UGameUserSettings::*GetFunc)() const;
	typedef void (UGameUserSettings::*SetFunc)(int);
	struct FSelectionElement
	{
		USelectionBase* Widget;
		GetFunc GetFunc;
		SetFunc SetFunc;
	
	};
}

void UVideoSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameUserSettings = GEngine->GetGameUserSettings();
	check(GameUserSettings);
	InitializeSync();
	InitializeFrameRate();
	InitializeResolutionComboBox();
	check(ShadowQuality);
	const FSelectionElement SelectionElement [] =
	{
		{ ShadingQuality, &UGameUserSettings::GetShadingQuality, &UGameUserSettings::SetShadingQuality },
		{ GlobalIllumination, &UGameUserSettings::GetGlobalIlluminationQuality, &UGameUserSettings::SetGlobalIlluminationQuality },
		{ PostProcessing, &UGameUserSettings::GetPostProcessingQuality, &UGameUserSettings::SetPostProcessingQuality },
		{ VisualEffectQuality, &UGameUserSettings::GetVisualEffectQuality, &UGameUserSettings::SetVisualEffectQuality },
		{ ShadowQuality, &UGameUserSettings::GetShadowQuality, &UGameUserSettings::SetShadowQuality }
	};
	for (const auto& [Widget, getFunc,setFunc] : SelectionElement)
	{
		const auto CurrentSelection = std::invoke(getFunc,GameUserSettings);
		Widget->SetCurrentSelection(CurrentSelection);
		Widget->OnSelectionChange.BindLambda([this,setFunc](int InSelection)
		{
			std::invoke(setFunc,GameUserSettings,InSelection);
			GameUserSettings->ApplySettings(false);
		}) ;
	}
}

void UVideoSettingsWidget::OnResolutionChanged(FString InItemSelected, ESelectInfo::Type InSelectionType)
{
	const auto SelectedResolution = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	GameUserSettings->SetScreenResolution(SelectedResolution);
	GameUserSettings->ApplySettings(false);
}

void UVideoSettingsWidget::InitializeResolutionComboBox()
{
	Resolutions.Reset();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	ResolutionComboBox->ClearOptions();
	for (const auto& resolution : Resolutions)
	{
		//const auto ResolutionString = resolution.ToString();
		const auto ResolutionString = FString::Printf(TEXT("%dx%d"),resolution.X, resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}
	const auto CurrentResolution = GameUserSettings->GetScreenResolution();/*
	const auto SelectIndex = Resolutions.IndexOfByPredicate([&CurrentResolution](const FIntPoint& InResolution)
	{
		return InResolution == CurrentResolution;
	});
	check(SelectIndex >= 0);*/
	ResolutionComboBox->SetSelectedIndex(1);
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this,&ThisClass::OnResolutionChanged);
}

void UVideoSettingsWidget::OnVSyncChanged(bool bIsChecked)
{
	GameUserSettings->SetVSyncEnabled(bIsChecked);
	GameUserSettings->ApplySettings(false);
}

void UVideoSettingsWidget::InitializeSync()
{
	VSyncCheckBox->SetIsChecked(false);
	VSyncCheckBox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this,&ThisClass::OnVSyncChanged);
}

void UVideoSettingsWidget::OnFrameRateChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameUserSettings->SetFrameRateLimit(FrameRates[SelectedItem]);
}

void UVideoSettingsWidget::InitializeFrameRate()
{
	FrameRateComboBox->ClearOptions();
	for (const auto& framerate : FrameRates)
	{
		FrameRateComboBox->AddOption(framerate.Key);
	}
	FrameRateComboBox->SetSelectedIndex(1);
	FrameRateComboBox->OnSelectionChanged.AddDynamic(this,&ThisClass::OnFrameRateChanged);
}
