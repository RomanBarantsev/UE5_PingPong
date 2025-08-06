// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PongPlayerState.h"
#include "ClassicPongPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API AClassicPongPlayerState : public APongPlayerState
{
	AClassicPongPlayerState();
	virtual void ChangeScore(int value) override;
	GENERATED_BODY()
};
