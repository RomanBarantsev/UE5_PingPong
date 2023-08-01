// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PingPongPlayerController.h"
#include "PingPongGameMode.generated.h"

class APingPongPlayerPawn;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName)
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
	
protected:
	APingPongGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION()
	APingPongPlayerPawn* CreatePawnForController(APingPongPlayerController* PingPongPlayerController,UWorld* World);
	UFUNCTION()
	void SetPawnRotationAndLocation(APingPongPlayerPawn* PingPongPlayerPawn,APingPongPlayerController* PingPongPlayerController);
	UFUNCTION()
	void SetClosestGoalOwner(APingPongPlayerPawn* PingPongPlayerPawn);

public:
	UFUNCTION()
	TArray<APingPongPlayerController*>& GetPlayersControllers();
	UFUNCTION()
	int GetPlayersCount() const;
};
