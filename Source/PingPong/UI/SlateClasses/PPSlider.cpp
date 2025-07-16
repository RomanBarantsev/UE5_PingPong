// Fill out your copyright notice in the Description page of Project Settings.


#include "PPSlider.h"

void UPPSlider::OnNativeCaptureEnd()
{	
		OnCppMouseCaptureEnd.Broadcast(AssociatedClass,GetValue());
}

void UPPSlider::SynchronizeProperties()
{
	if (!OnMouseCaptureEnd.IsAlreadyBound(this, &UPPSlider::OnNativeCaptureEnd))
	{
		OnMouseCaptureEnd.AddDynamic(this, &UPPSlider::OnNativeCaptureEnd);
	}	
	Super::SynchronizeProperties();
}
