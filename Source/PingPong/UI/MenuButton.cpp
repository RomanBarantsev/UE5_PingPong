// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuButton.h"

#include "FStyleSet.h"
#include "PPSlateWidgetStyle.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UMenuButton::NativePreConstruct()
{
	auto SlateStyle  =  FStyleSet::Get().GetWidgetStyle<FPPSlateStyle>(FName("PPSlateWidgetStyle"));
	Button->SetStyle(SlateStyle.ButtonStyle);
	Super::NativePreConstruct();
}

void UMenuButton::NativeConstruct()
{
	Button->OnClicked.AddDynamic(this,&UMenuButton::HandleButtonClicked);
	Super::NativeConstruct();
}

void UMenuButton::HandleButtonClicked()
{
	OnButtonClicked.Broadcast();
}

void UMenuButton::SetButtonImage(UTexture2D* Texture)
{
	/*if (!Button || !Texture) return;
	FSlateBrush Brush;
	Brush.SetResourceObject(Texture);
	Brush.ImageSize = FVector2D(480.f, 480.f); // Adjust based on your PNG
	Brush.DrawAs = ESlateBrushDrawType::Image;
	FButtonStyle ButtonStyle;
	ButtonStyle.Normal = Brush;
	Button->SetStyle(ButtonStyle);
	Text->SetText(FText::FromString("Text"));*/
}

void UMenuButton::SetButtonText(FString txt)
{
	Text->SetText(FText::FromString(txt));
}