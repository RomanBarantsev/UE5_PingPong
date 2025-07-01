// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/UI/HUDs/GameHUD.h"
#include "PingPongPlayerController.generated.h"

class APingPongPlayerState;
class ABaseHUD;
UENUM()
enum class EUIStatus : uint8
{
	NONE,
	UILoaded,
	ReadyButtonPressed,
	UIPaused,
	Started
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
	TMap<uint32, FString> PlayerList;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY()
	FTransform StartTransform;
	UPROPERTY()
	APingPongGameState* PingPongGameState;
	UPROPERTY()
	APingPongPlayerState* PingPongPlayerState;
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
	UPROPERTY()
	bool bBlockMovement=true;
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
public:
	UFUNCTION(Client,Reliable,WithValidation)
	void OpenMenu();
	UFUNCTION(Server,Reliable,WithValidation)
	void RequstPause(bool state);
	
protected:
	UPROPERTY()
	AGameHUD* PingPongHUD;
	EUIStatus UIStatus;
	
public:
	UFUNCTION(Server,Reliable)
	void SetUIStatus(EUIStatus status);
	UFUNCTION(Client,Reliable)
	void SetNewScore(int32 PlayerId, float Score);
	UFUNCTION()
	TMap<uint32,FString> GetPlayersInGame();
private:
	int32 ModificationsCount;	
	UFUNCTION(Server,Reliable,WithValidation)
	void ScrollColorOnServer(float Axis);
	UFUNCTION()
	void ScrollColor(float Axis);
	UFUNCTION(Client,Reliable)
	void SetColorAndPriceUI(FLinearColor Color,int32 Price);

private:
	bool bIsMovingSides=false;
	bool bIsMovingForward=false;
	
	UFUNCTION()
	void HandleMatchStateChange(FName NewState);
};
