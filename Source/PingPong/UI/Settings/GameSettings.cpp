// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings.h"

#include "PingPong/PlayerControllers/PingPongPlayerController.h"

void UGameSettings::TextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	auto PS =GetOwningPlayerState(true);
	PS->SetPlayerName(Text.ToString());
}

void UGameSettings::NativeConstruct()
{	
	Nickname->OnTextCommitted.AddDynamic(this,&ThisClass::TextCommited);
	Super::NativeConstruct();
}
