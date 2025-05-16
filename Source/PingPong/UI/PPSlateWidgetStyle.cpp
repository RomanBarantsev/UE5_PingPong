// Fill out your copyright notice in the Description page of Project Settings.


#include "PPSlateWidgetStyle.h"

FPPSlateStyle::FPPSlateStyle()
{
	CheckBoxStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("CheckBox");
	TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");
}

FPPSlateStyle::~FPPSlateStyle()
{
}

const FName FPPSlateStyle::TypeName(TEXT("FPPSlateStyle"));

const FPPSlateStyle& FPPSlateStyle::GetDefault()
{
	static FPPSlateStyle Default;
	return Default;
}

void FPPSlateStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

