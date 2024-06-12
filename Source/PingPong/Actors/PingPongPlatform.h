// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPong/ActorComponents/PingPongBallPool.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PingPongPlatform.generated.h"

class APingPongPlayerController;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class PINGPONG_API APingPongPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APingPongPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category =	"Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category =	"Components")
	UStaticMeshComponent* MeshRoot;	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UArrowComponent* ShootDirectionArrow;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UPlatformModificator* PlatformModificator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPingPongBallPool* BallsPoolComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 15;
	float InterpolationKey = 0.02f;
	float CurrentRightAxisValue;
	float targetRightAxisValue;
	float CurrentForwardAxisValue;
	float targetForwardAxisValue;
	
public:
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveRight(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveForward(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_Rotate(float AxisValue);
	UFUNCTION(Server,Reliable, WithValidation)
	void Server_Fire(EBallModificators Modificator);
	UPROPERTY(EditAnywhere)
	TSubclassOf<APingPongBall> BallClass;
	
protected:
	float InitialZ=0;
	float Amplitude=10.0f;;
	float Frequency=1;
	float RunningTime=1;
public:
	UFUNCTION(Server,Unreliable)	
	void Floating();
	UPROPERTY(EditAnywhere)
	bool bFloating = true; 
	
protected:	
	UPROPERTY(Replicated)
	float AxisMoveValue = 0;
	float StartRotatePos;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Client,Reliable)
	void SetSoundPitch(float pitch);
	
public:
	UPROPERTY()
	bool bInvertedControl = false;
	UFUNCTION()
	void SetSpeedMultiplier(int32 Multiplier);
	UFUNCTION()
	bool CheckScore(EBallModificators Modificator);

private:
	UPROPERTY()
	APingPongGameState* GameState;
};
