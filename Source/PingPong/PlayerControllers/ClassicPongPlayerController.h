// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PongPlayerController.h"
#include "ClassicPongPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API AClassicPongPlayerController : public APongPlayerController
{
	GENERATED_BODY()
	virtual void RotatePlatform(float AxisValue) override;
	virtual void MoveForward(float AxisValue) override;
	virtual void Fire() override;
	virtual void SetupInputComponent() override;
};
