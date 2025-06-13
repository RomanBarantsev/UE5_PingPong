// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"


void USettings::OnExitBtnPressed()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void USettings::OnGameBtnPushed()
{
	if (WidgetSwitcher && VideoSettingsWidget)
	{
		WidgetSwitcher->SetActiveWidget(GameSettingsWidget);
	}
}

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
	if (GameSettingsSubClass)
		{
			GameSettingsWidget = CreateWidget<UUserWidget>(GetWorld(), GameSettingsSubClass); // ✅ Correct class
			WidgetSwitcher->AddChild(GameSettingsWidget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No GameSettingsSubClass in Settings Widget"));
			UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
		}

	Sound->OnPressed.AddUniqueDynamic(this, &USettings::OnSoundBtnPushed);
	Video->OnPressed.AddUniqueDynamic(this, &USettings::OnVideoBtnPushed);
	Game->OnPressed.AddUniqueDynamic(this, &USettings::OnGameBtnPushed);
	Exit->OnPressed.AddUniqueDynamic(this, &USettings::OnExitBtnPressed);
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
