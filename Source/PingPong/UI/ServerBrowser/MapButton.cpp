// Fill out your copyright notice in the Description page of Project Settings.


#include "MapButton.h"

#include "HostGame.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"

void UMapButton::ChooseBtnClicked()
{
	OnMapButtonClicked.Broadcast(MapName);
}

void UMapButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (SizeBox)
	{
		SizeBox->ClearMinDesiredWidth();
		SizeBox->ClearMinDesiredHeight();
		SizeBox->SetMinDesiredWidth(150.0f);
		SizeBox->SetMinDesiredHeight(150.0f);
	}
}

void UMapButton::NativeConstruct()
{
	
	SizeBox->AddChild(ChooseBtn);
	ChooseBtn->OnClicked.AddDynamic(this,&UMapButton::ChooseBtnClicked);
	Super::NativeConstruct();
}
