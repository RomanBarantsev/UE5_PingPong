// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuInstance.h"

#include "MenuSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"

void UMenuInstance::OnApplyBtnPushed()
{
	
}

void UMenuInstance::OnExitBtnPushed()
{
	
}

void UMenuInstance::NativeConstruct()
{
	if (MenuItemClass)
	{
		UUserWidget* MenuItem = CreateWidget<UMenuSlot>(GetWorld(),MenuItemClass);
		HorizontalBox->AddChild(MenuItem);
	}
	// Create Apply Button
	ApplyBtn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ApplyButton"));
	UTextBlock* ApplyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	ApplyText->SetText(FText::FromString("Apply"));
	ApplyBtn->AddChild(ApplyText);

	// Create Exit Button
	ExitBtn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ExitButton"));
	UTextBlock* ExitText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	ExitText->SetText(FText::FromString("Exit"));
	ExitBtn->AddChild(ExitText);

	// Create horizontal container for buttons
	UHorizontalBox* ButtonBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	ButtonBox->AddChildToHorizontalBox(ApplyBtn);
	ButtonBox->AddChildToHorizontalBox(ExitBtn);

	// Create root overlay
	UOverlay* RootOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
	this->WidgetTree->RootWidget = RootOverlay;

	// Add the button box to the overlay
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(RootOverlay->AddChild(ButtonBox));
	if (OverlaySlot)
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Right);
		OverlaySlot->SetVerticalAlignment(VAlign_Bottom);
		OverlaySlot->SetPadding(FMargin(10.f));
	}
	Super::NativeConstruct();
}

