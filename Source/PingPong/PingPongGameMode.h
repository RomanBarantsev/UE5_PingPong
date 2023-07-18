// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
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
	class APingPongPlayerController* Player1 = NULL;
	UPROPERTY()
	class APingPongPlayerController* Player2 = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player1Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player2Start;
	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;
	int PlayersCount=2;
	int ScoreToEnd=100;
	
public:
	APingPongGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION()
	APlayerController* GetPlayerController(int index);
	FOnMatchStateChanged OnMatchStateChanged;
	
public:
	UFUNCTION()
	void PlayerReady();	
private:
	int32 PlayersReady=0;
};
