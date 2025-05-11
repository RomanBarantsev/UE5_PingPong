// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuVideo.h"

#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"


void UMenuVideo::NativeConstruct()
{
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	ResolutionComboBox = NewObject<UComboBoxString>(this);
	for (const FIntPoint& Res : Resolutions)
	{
		FString ResStr = FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);
		ResolutionComboBox->AddOption(ResStr);
	}	
	//HorizontalBox->AddChild()
	Super::NativeConstruct();
}
