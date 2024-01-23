// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPongPlayerState.generated.h"

class APingPongGameState;
/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	bool PlayerReady=false;
	int ShootBalls = 0;
	UPROPERTY()
	APingPongGameState* GameState;
	EBallModificators Modificator = EBallModificators::None;

public:
	UFUNCTION()
	EBallModificators GetModificator();
	UFUNCTION()
	void SetModificator(EBallModificators modificator);
};
