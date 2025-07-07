// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameState.h"
#include "PongGameState.generated.h"

class APongPlayerState;
class APostProcessVolume;
class APongPlayerController;
class APongGameMode;

UENUM()
enum class EBallModificators : int8	
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
struct FBallModificatorsTable :public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EBallModificators BallModificators = EBallModificators::None;
	UPROPERTY(EditAnywhere)
	FLinearColor Color=FLinearColor::Black;
	UPROPERTY(EditAnywhere)
	int32 Points=1;
	UPROPERTY(EditAnywhere)
	int32 ShotCost=1;
	UPROPERTY(EditAnywhere)
	float Speed=800;
	UPROPERTY(EditAnywhere)
	float ModificatorValue=0;
	UPROPERTY(EditAnywhere)
	bool isUseByPlayer=true;
	UPROPERTY(EditAnywhere)
	FString Note{"Explanation"};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName, NewState);
/**
 * 
 */
UCLASS()
class PINGPONG_API APongGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	APongGameState();
	virtual void BeginPlay() override;	
	UFUNCTION()
	void OnrepPlayerStatesUpdated();
public:
	UFUNCTION()
	void HandlePlayerStatesUpdated();
	UPROPERTY(ReplicatedUsing="OnrepPlayerStatesUpdated");
	TArray<APlayerState*> PlayerStates;
	
	int ScoreToEnd=100;
public:
	UFUNCTION(Category="Score")
	void AddMaxScore(int Score);
private:
	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;	
	UPROPERTY(Replicated)
	int32 ReadyPlayers{0};
	UPROPERTY(Replicated)
	int32 LoadedPlayers;
	UPROPERTY(Replicated)
	int32 StartedPlayers;
	UPROPERTY()
	int32 CountDown {3};
public:
	UFUNCTION()
	int32 GetCountDownTime();

private:
	UFUNCTION(Client,Reliable)
	void SetCountDownOnPlayerSide();
	UPROPERTY()
	const APongGameMode* GameMode;
public:
	UFUNCTION(Server,Reliable)
	void IncreaseReadyPlayer();
	UFUNCTION(Server,Reliable)
	void IncreaseLoadedPlayer();
	UFUNCTION(Server,Reliable)
	void IncreaseStartedPlayers();
	UFUNCTION(Server,Reliable)
	void DecreaseLoadedPlayer(AController* PC);
	UFUNCTION(Server,Reliable)	
	void ServerPause(bool state);
	UFUNCTION(NetMulticast,Reliable)
	void UpdatePlayersScore(int32 playerId, int32 Score);	
	
private:
	UFUNCTION()
	void CalculateEnumBallModifications();
	UPROPERTY()
	int32 EBallModificatorsCount;
	UPROPERTY()
	TMap<int8,EBallModificators> ModificationMap;
public:
	UFUNCTION()
	int32 GeBallModificatorsCount();
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* BallModificatorsDataTable;
	TArray<FName> BallModificatorsRowNames;
public:
	UFUNCTION()
	EBallModificators GetModifcation(int8 mod);
	UFUNCTION()
	FLinearColor GetModificatorColor(EBallModificators modificator);
	UFUNCTION()
	int32 GetModificatorPoints(EBallModificators modificator);
	UFUNCTION()
	int32 GetShotCost(EBallModificators modificator);
	UFUNCTION()
	int32 GetBallSpeed(EBallModificators modificator);
	UFUNCTION()
	float GetBallModificationValue(EBallModificators modificator);
	FBallModificatorsTable* GetModificationRow(EBallModificators Modificator);	
	UPROPERTY(BlueprintAssignable, Category = "Match State")
	FOnMatchStateChanged OnMatchStateChanged;
	
	UFUNCTION(NetMulticast,Reliable)
	void SetMatchState(FName NewState);
	FName GetMatchState() const;
};
