// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerScore.h"

#include <string>

#include "Kismet/KismetSystemLibrary.h"

void UPlayerScore::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerScore::SetPlayerName(FString Name)
{
	PlayerName->SetText(FText::FromString(Name));
}

void UPlayerScore::SetPlayerScore(int Score)
{	
	ScoreText->SetText(FText::AsNumber(Score));
}
