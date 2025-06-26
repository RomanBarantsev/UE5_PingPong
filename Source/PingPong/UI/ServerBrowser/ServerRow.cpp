// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Engine/Canvas.h"

void UServerRow::SetCurrentPlayers(int Players)
{
	PlayersTxt->SetText(FText::AsNumber(Players));
}

void UServerRow::SetServerName(FString Name)
{
	ServerNameTxt->SetText(FText::FromString(*Name));
}

void UServerRow::NativeConstruct()
{
	Super::NativeConstruct();
}
