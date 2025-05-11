// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuButton.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UMenuButton::NativePreConstruct()
{
	/*SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(),TEXT("SizeBox"));
	SizeBox->SetHeightOverride(480);
	SizeBox->SetWidthOverride(480);
	WidgetTree->RootWidget = SizeBox;*/
	/*Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Button"));
	SizeBox->AddChild(Button);
	Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Text"));
	Button->AddChild(Text);*/
	Button->OnClicked.AddDynamic(this,&UMenuButton::HandleButtonClicked);
	//SetButtonImage(LoadObject<UTexture2D>(nullptr, TEXT("/Game/PingPong/UI/IMG/2.2")));
	Super::NativePreConstruct();
}

void UMenuButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMenuButton::HandleButtonClicked()
{
	OnButtonClicked.Broadcast();
}

void UMenuButton::SetButtonImage(UTexture2D* Texture)
{
	if (!Button || !Texture) return;
	FSlateBrush Brush;
	Brush.SetResourceObject(Texture);
	Brush.ImageSize = FVector2D(480.f, 480.f); // Adjust based on your PNG
	Brush.DrawAs = ESlateBrushDrawType::Image;
	FButtonStyle ButtonStyle;
	ButtonStyle.Normal = Brush;
	Button->SetStyle(ButtonStyle);
	Text->SetText(FText::FromString("Text"));
}

void UMenuButton::SetButtonText(FString txt)
{
	Text->SetText(FText::FromString(txt));
}