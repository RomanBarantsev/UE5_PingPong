// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PingPongGameMode.generated.h"

class APingPongGameState;
class APingPongPlayerController;
class APingPongPlayerPawn;
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
	TArray<APlayerStart*> PlayerStarts;
	int PlayersCount=2;
	
protected:
	APingPongGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	UFUNCTION()
	APingPongPlayerPawn* CreatePawnForController(APingPongPlayerController* PingPongPlayerController,UWorld* World);
	UFUNCTION(Server,Reliable)
	void SetPawnRotationAndLocation(APingPongPlayerPawn* PingPongPlayerPawn,APingPongPlayerController* PingPongPlayerController);
	UFUNCTION()
	void SetClosestGoalOwner(APingPongPlayerPawn* PingPongPlayerPawn);
	UPROPERTY()
	APingPongGameState* PingPongGameState;
public:
	
	UFUNCTION()
	int GetPlayersCount() const;
	
};
