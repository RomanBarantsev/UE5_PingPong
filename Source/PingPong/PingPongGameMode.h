// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PingPongPlayerController.h"
#include "PingPongGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged,FName)
class APlayerStart;
/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameMode : public AGameMode
{
	GENERATED_BODY()
protected:	
	UPROPERTY()
	TArray<APingPongPlayerController*> PlayerControllers;
	UPROPERTY()
	TArray<APlayerStart*> PlayerStarts;
	int PlayersCount=2;
	
public:
	APingPongGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
private:
	int32 PlayersReady=0;	
};
