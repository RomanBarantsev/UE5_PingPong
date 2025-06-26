// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "IAutomationControllerManager.h"
#include "Components/Button.h"
#include "Engine/Canvas.h"

void UServerRow::SetCurrentPlayers(int Players,int maxPlayers)
{
	FText text = (FText::FromString(FString::FromInt(Players)+"/"+FString::FromInt(maxPlayers)));
	PlayersTxt->SetText(text);
}

void UServerRow::SetServerName(FString Name)
{
	ServerNameTxt->SetText(FText::FromString(*Name));
}

void UServerRow::OnButtonClicked()
{
	auto PC = GetOwningPlayer();
	if (PC)
	{
		const FString URL = FString::Printf(TEXT("%s:%d"), *IP, port);
		PC->ClientTravel(URL,TRAVEL_Absolute);
	}
}

void UServerRow::NativeConstruct()
{
	Button->OnClicked.AddDynamic(this,&ThisClass::OnButtonClicked);
	Super::NativeConstruct();
}
