// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Components/Button.h"
#include "Engine/Canvas.h"

void UServerRow::SetCurrentPlayers(int Players,int maxPlayer)
{
	PlayersTxt->SetText(FText::AsNumber(Players));
}

void UServerRow::SetServerName(FString Name)
{
	ServerNameTxt->SetText(FText::FromString(*Name));
}

void UServerRow::OnButtonClicked()
{
	
}

void UServerRow::NativeConstruct()
{
	Button->OnClicked.AddDynamic(this,&ThisClass::OnButtonClicked);
	Super::NativeConstruct();
}
