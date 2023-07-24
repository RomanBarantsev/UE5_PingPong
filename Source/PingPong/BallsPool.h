// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PingPongBall.h"
#include "UObject/NoExportTypes.h"
#include "BallsPool.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UBallsPool : public UActorComponent
{
	GENERATED_BODY()

protected:
	UBallsPool();
	int MaxPoolSize = 20;
	UPROPERTY()
	TArray<APingPongBall*> PingPongBalls;
	UFUNCTION()
	void FillPool();
	
};
