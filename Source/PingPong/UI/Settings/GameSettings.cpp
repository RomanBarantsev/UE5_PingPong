// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings.h"

#include "Kismet/GameplayStatics.h"
#include "PingPong/Pong_GameUserSettings.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void UGameSettings::TextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	auto PS =GetOwningPlayerState(true);
	PS->SetPlayerName(Text.ToString());
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings)
	{
		auto Pong_Settings = Cast<UPong_GameUserSettings>(Settings);
		check(Pong_Settings);
		Pong_Settings->PlayerName = Text.ToString();
		Pong_Settings->SaveConfig();
	}
}

void UGameSettings::NativeConstruct()
{	
	Nickname->OnTextCommitted.AddDynamic(this,&ThisClass::TextCommited);
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings)
	{
		auto Pong_Settings = Cast<UPong_GameUserSettings>(Settings);
		check(Pong_Settings);
		APlayerState* PlayerState = UGameplayStatics::GetPlayerState(GetWorld(),0);
		if (PlayerState)
		{
			Pong_Settings->LoadConfig();
			PlayerState->SetPlayerName(Pong_Settings->PlayerName);			
		}		
	}
	auto PS =GetOwningPlayerState(true);
	if (PS)
		Nickname->SetText(FText::FromString(PS->GetPlayerName()));
	Super::NativeConstruct();
}
