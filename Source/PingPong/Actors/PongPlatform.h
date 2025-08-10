// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPong/ActorComponents/PongBallPool.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PongPlatform.generated.h"

class APongPlayerController;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class PINGPONG_API APongPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongPlatform();

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
	UPongBallPool* BallsPoolComponent;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 1000;
	float MoveSpeedMax{0};
	float MoveSpeedMin {0};
	float MoveSpeedMultiplier=2;
	float InterpolationKey = 0.5f;
	float interpolatedYaw;
	float CurrentRightAxisValue;
	float targetRightAxisValue;
	float CurrentForwardAxisValue;
	float targetForwardAxisValue;
	FVector TargetLocation;
	FVector currentLocation;
	UPROPERTY()
	APongGameState* PingPongGameState;
public:
	float GetMoveSpeed();
    void TickMoveRight(float DeltaTime);
    void TickMoveForward(float DeltaTime);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_GetRightValue(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_GetForwardValue(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_Rotate(float AxisValue);
	UFUNCTION(Server,Reliable, WithValidation)
	void Server_Fire(EBallModificators Modificator);
	UPROPERTY(EditAnywhere)
	float RotateAngle=45;
	UFUNCTION()
	UPlatformModificator* GetPlatformModificator() const;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APongBall> BallClass;
	
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
	float StartRotatePos;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Client,Reliable)
	void SetSoundPitch(float pitch);
	
public:
	UPROPERTY()
	bool bInvertedControl = false;
	UFUNCTION()
	void SetSpeedMultiplier(float Multiplier);
	UFUNCTION()
	bool CheckScore(EBallModificators Modificator);

private:
	UPROPERTY()
	APongGameState* GameState;
};
