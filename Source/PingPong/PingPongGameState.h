// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameState.h"
#include "PingPongGameState.generated.h"

class APingPongPlayerController;
class APingPongGameMode;
UENUM()
enum class EPlayersStatus
{	
	AllPlayersConnected UMETA(DisplayName="AllPlayerConnected"),
	AllPlayersIsReady UMETA(DisplayName="AllPlayersIsReady"),
	NONE UMETA(DisplayName="None")
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayersStateChanged,EPlayersStatus)

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	APingPongGameState();
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void UpdateCharacterState(EPlayersStatus NewPlayersState);
	UFUNCTION()
	EPlayersStatus GetPlayersStatus() const;
	UPROPERTY(Replicated,BlueprintReadWrite)
    TArray<APingPongPlayerController*> PlayerControllers;
	
	FOnPlayersStateChanged OnPlayersStateChanged;
private:
	UPROPERTY(Replicated)
	EPlayersStatus CurrentPlayersState;
	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	int ScoreToEnd;
	UPROPERTY(Replicated)
	int32 ReadyPlayers;
	UPROPERTY(Replicated)
	int32 LoadedPlayers;
	UPROPERTY()
	const APingPongGameMode* GameMode;
public:
	UFUNCTION(Server,Reliable)
	void IncreaseReadyPlayer();
	UFUNCTION(Server,Reliable)
	void IncreaseLoadedPlayer();
	UFUNCTION()
	TArray<APingPongPlayerController*>& GetPlayersControllers();
};
