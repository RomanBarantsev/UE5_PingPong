// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSlot.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UMenuSlot::NativeConstruct()
{
	HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("MyHorizontalBox"));
	WidgetTree->RootWidget = HorizontalBox;

	TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MyTextBlock"));
	TextBlock->SetText(FText::FromString("Hello Menu"));

	HorizontalBox->AddChildToHorizontalBox(TextBlock);
	Super::NativeConstruct();
}
