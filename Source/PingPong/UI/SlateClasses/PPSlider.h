// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "PPSlider.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCppMouseCaptureEnd, USoundClass*,float);
/**
 * 
 */
UCLASS()
class PINGPONG_API UPPSlider : public USlider
{
	GENERATED_BODY()
public:
	UPROPERTY()
	USoundClass* AssociatedClass;
	FOnCppMouseCaptureEnd  OnCppMouseCaptureEnd;
	UFUNCTION()
	void OnNativeCaptureEnd();	
	virtual void SynchronizeProperties() override;
	
};
