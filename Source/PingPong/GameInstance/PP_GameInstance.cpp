// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"

#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerState.h"
#include "PingPong/Pong_GameUserSettings.h"

class UPong_GameSettings;

void UPP_GameInstance::Init()
{
	/*if (GEngine)
	{
		GEngine->GameUserSettings = NewObject<UPong_GameUserSettings>();
		GEngine->GameUserSettings->LoadSettings(true);
	}*/
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings)
	{
		Settings->LoadConfig();	
		Pong_Settings = Cast<UPong_GameUserSettings>(Settings);
		check(Pong_Settings);	
	}
	Super::Init();
}

void UPP_GameInstance::Shutdown()
{		
	Super::Shutdown();
}

void UPP_GameInstance::LoadAudioSettings()
{
	
}

void UPP_GameInstance::LoadVideoSettings()
{
	
}
