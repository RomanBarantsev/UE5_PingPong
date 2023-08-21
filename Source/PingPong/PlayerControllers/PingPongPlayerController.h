// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"
class APingPongHUD;
UENUM()
enum class EUIStatus : uint8
{
	NONE,
	UILoaded,
	ReadyButtonPressed,
	UIPaused
};
enum class EPlayersStatus;
class APingPongGameState;
/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY()
	FTransform StartTransform;
	UPROPERTY()
	APingPongGameState* PingPongGameState;
	UFUNCTION(Server,Reliable)
	void OnPlayersStateChanged(EPlayersStatus PlayersStatus);
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class APingPongPlatform> PlatformClass;
	UPROPERTY()
	class APingPongPlatform* Platform;

public:
	APingPongPlayerController();
	UFUNCTION()
	void SetStartTransform(FTransform NewStartTransform);
	UFUNCTION(Server,Reliable,WithValidation)
	void Initialize();
	virtual void PreInitializeComponents() override;	
	UFUNCTION(Server,Reliable,WithValidation)
	void SpawnPlatform();
	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
    void RotatePlatform(float AxisValue);    	
	UFUNCTION(Server,Reliable,WithValidation)
	void Fire();
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlatformMoveRight(float AxisValue);
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlatformMoveForward(float AxisValue);
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlatformRotate(float AxisValue);

protected:
	UPROPERTY()
	APingPongHUD* PingPongHUD;
	UFUNCTION()
	void OpenMenu();
	EUIStatus UIStatus;
	
public:
	UFUNCTION(Server,Reliable)
	void SetUIStatus(EUIStatus status);
	UFUNCTION(Client,Reliable)
	void AllPlayersConnected();
	UFUNCTION(Client,Reliable)
	void AllPlayersReady(int32 CountDownValue);
	UFUNCTION(Client,Reliable)
	void SetNewScore(int32 PlayerId, float Score);
	UFUNCTION(Client,Reliable)
	void SetScoreText(int32 PlayerId);

};
