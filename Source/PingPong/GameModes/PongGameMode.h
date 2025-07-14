// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PongGameMode.generated.h"

class APongGameState;
class APongPlayerController;
class APongPlayerPawn;
class APlayerStart;
/**
 * 
 */
UCLASS()
class PINGPONG_API APongGameMode : public AGameMode
{
	GENERATED_BODY()
protected:	
	
	UPROPERTY()
	TArray<APlayerStart*> PlayerStarts;
	int PlayersCount=2;
	
protected:
	APongGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	UFUNCTION()
	APongPlayerPawn* CreatePawnForController(APongPlayerController* PingPongPlayerController,UWorld* World);
	UFUNCTION(Server,Reliable)
	void SetPawnRotationAndLocation(APongPlayerPawn* PingPongPlayerPawn,APongPlayerController* PingPongPlayerController);
	UFUNCTION()
	void SetClosestGoalOwner(APongPlayerPawn* PingPongPlayerPawn);
	UPROPERTY()
	APongGameState* PingPongGameState;
public:
	
	UFUNCTION()
	int GetPlayersCount() const;
	
};
