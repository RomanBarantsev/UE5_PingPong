// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings.h"

#include "MenuAudio.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"


void USettings::NativeConstruct()
{	
	if (AudioSettingsSubClass)
	{
		AudioSettingsWidget = CreateWidget<UUserWidget>(GetWorld(), AudioSettingsSubClass);
		WidgetSwitcher->AddChild(AudioSettingsWidget);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No AudioSettingsSubClass in Settings Widget"));
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	}

	if (VideoSettingsSubClass)
	{
		VideoSettingsWidget = CreateWidget<UUserWidget>(GetWorld(), VideoSettingsSubClass); // ✅ Correct class
		WidgetSwitcher->AddChild(VideoSettingsWidget);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No VideoSettingsSubClass in Settings Widget"));
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	}

	Sound->OnPressed.AddUniqueDynamic(this, &USettings::OnSoundBtnPushed);
	Video->OnPressed.AddUniqueDynamic(this, &USettings::OnVideoBtnPushed);
	Super::NativeConstruct();
}

void USettings::OnVideoBtnPushed()
{
	if (WidgetSwitcher && VideoSettingsWidget)
	{
		WidgetSwitcher->SetActiveWidget(VideoSettingsWidget);
	}
}

void USettings::OnSoundBtnPushed()
{
	if (WidgetSwitcher && AudioSettingsWidget)
	{
		WidgetSwitcher->SetActiveWidget(AudioSettingsWidget);
	}
}
