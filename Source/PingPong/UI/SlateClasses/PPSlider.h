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
public:
	UPROPERTY()
	USoundClass* AssociatedClass;
	GENERATED_BODY()
	FOnCppMouseCaptureEnd  OnCppMouseCaptureEnd;
	virtual void SynchronizeProperties() override;
	UFUNCTION()
	void OnNativeCaptureEnd()
	{
		OnCppMouseCaptureEnd.Broadcast(AssociatedClass,GetValue());
	}
};
