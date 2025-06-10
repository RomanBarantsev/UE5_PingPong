// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings.h"

#include "Kismet/GameplayStatics.h"

void UGameSettings::TextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	APlayerState* PS = PC->PlayerState;
	if (PS)
	{
		PS->SetPlayerName(Text.ToString());
	}	
}

void UGameSettings::NativeConstruct()
{
	Nickname->OnTextCommitted.AddDynamic(this,&ThisClass::TextCommited);
	Super::NativeConstruct();
}
