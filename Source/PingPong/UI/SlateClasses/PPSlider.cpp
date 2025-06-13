// Fill out your copyright notice in the Description page of Project Settings.


#include "PPSlider.h"

void UPPSlider::SynchronizeProperties()
{
	OnMouseCaptureEnd.AddDynamic(this, &UPPSlider::OnNativeCaptureEnd);
	Super::SynchronizeProperties();
}
