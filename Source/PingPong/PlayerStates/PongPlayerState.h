// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PingPong/GameStates/PongGameState.h"
#include "PongPlayerState.generated.h"

class APongGameState;
/**
 * 
 */
UCLASS()
class PINGPONG_API APongPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	bool PlayerReady=false;
	int ShootBalls = 0;
	UPROPERTY()
	APongGameState* GameState;
	int8 maxModificator=0;
	int8 Modificator=0;

public:
	UFUNCTION()
	void NextModificator();
	UFUNCTION()
	void PrevModificator();
	UFUNCTION()
	EBallModificators GetModificator();
	UFUNCTION()
	virtual void ChangeScore(int value);
};
