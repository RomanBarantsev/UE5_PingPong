// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "PPSlateWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct PINGPONG_API FPPSlateStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPPSlateStyle();
	virtual ~FPPSlateStyle();

	/** Style of check boxes */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FCheckBoxStyle CheckBoxStyle;

	/** Style of options text */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FTextBlockStyle TextStyle;

	/* Button style */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FButtonStyle ButtonStyle;
	
	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPPSlateStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UPPSlateWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FPPSlateStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
