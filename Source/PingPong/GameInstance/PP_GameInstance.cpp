// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PingPong/Pong_GameUserSettings.h"

class UPong_GameSettings;

void UPP_GameInstance::Init()
{
	Settings = GEngine->GameUserSettings;
	if (Settings)
	{
		Settings->LoadConfig();	
		Pong_Settings = Cast<UPong_GameUserSettings>(Settings);
		check(Pong_Settings);	
	}
	else
	{
		UKismetSystemLibrary::QuitGame(GetWorld(),GetPrimaryPlayerController(),EQuitPreference::Quit,true);
		UKismetSystemLibrary::LogString(TEXT("No Settings class in GameInstance class"), true);
	}
	Super::Init();
}

void UPP_GameInstance::Shutdown()
{
	Settings->SaveConfig();	
	Super::Shutdown();
}
