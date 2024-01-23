// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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

UENUM()
enum class EBallModificators	
{	
	Fast UMETA(DisplayName="Fast"),
	Slow UMETA(DisplayName="Slow"),
	Shrink UMETA(DisplayName="Shrink"),
	Expand UMETA(DisplayName="Expand"),
	ReverseControl UMETA(DisplayName="ReverseControl"),
	LightsOff UMETA(DisplayName="LightsOff"),
	None UMETA(DisplayName="None")	
};

USTRUCT()
struct FBallModificators :public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EBallModificators BallModificators;
	UPROPERTY(EditAnywhere)
	FLinearColor Color;
	UPROPERTY(EditAnywhere)
	int32 Points;
	UPROPERTY(EditAnywhere)
	int32 ShotCost;
	float Speed;
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
	FOnPlayersStateChanged OnPlayersStateChanged;
private:	
	UPROPERTY(Replicated)
	TArray<APingPongPlayerController*> PlayerControllers;
	UPROPERTY(Replicated)
	EPlayersStatus CurrentPlayersState;
	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	int ScoreToEnd=100;
	UPROPERTY(Replicated)
	int32 ReadyPlayers=0;
	UPROPERTY(Replicated)
	int32 LoadedPlayers;	
	UPROPERTY()
	int32 CountDown = 1;	
	FTimerHandle CountDownHandle;
	UFUNCTION(Server,Reliable)
	void UpdateCountdown();
	UPROPERTY()
	const APingPongGameMode* GameMode;
public:
	UFUNCTION(Server,Reliable)
	void IncreaseReadyPlayer();
	UFUNCTION(Server,Reliable)
	void IncreaseLoadedPlayer();
	TArray<APingPongPlayerController*>& GetPlayersControllers();
	UFUNCTION(Client,Reliable)
	void UpdatePlayersScore(int32 playerId, int32 Score);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* BallModificatorsDataTable;
	TArray<FName> BallModificatorsRowNames;
public:
	UFUNCTION()
	FLinearColor GetModificatorColor(EBallModificators modificator);
	UFUNCTION()
	int32 GetModificatorPoints(EBallModificators modificator);
	UFUNCTION()
	int32 GetShotCost(EBallModificators modificator);
	FBallModificators* GetModificationRow(EBallModificators Modificator);
};
