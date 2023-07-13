// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameMode.generated.h"

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
public:
	APingPongGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION()
	APlayerController* GetPlayerController(int index);
	virtual bool IsPaused() const override;
	
};
