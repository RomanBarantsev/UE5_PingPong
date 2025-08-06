// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingPong/GameStates/PongGameState.h"
#include "ClassicPongGameState.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API AClassicPongGameState : public APongGameState
{
	GENERATED_BODY()
	AClassicPongGameState();
	virtual void AddMaxScore(int Score) override;
public:
	void ResetScore();
};
