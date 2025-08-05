// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidget.h"
#include "ClassicOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UClassicOverlayWidget : public UOverlayWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
};
