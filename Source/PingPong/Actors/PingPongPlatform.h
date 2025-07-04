// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPong/ActorComponents/PongBallPool.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PingPongPlatform.generated.h"

class APongPlayerController;
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
	UPongBallPool* BallsPoolComponent;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 2000;
	float InterpolationKey = 0.05f;
	float interpolatedYaw;
	float CurrentRightAxisValue;
	float targetRightAxisValue;
	float CurrentForwardAxisValue;
	float targetForwardAxisValue;
	FVector TargetLocation;
	FVector currentLocation;
public:
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Server_MoveRight(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Server_MoveForward(float DeltaTime);
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
	void SetSpeedMultiplier(int32 Multiplier);
	UFUNCTION()
	bool CheckScore(EBallModificators Modificator);

private:
	UPROPERTY()
	APongGameState* GameState;
};
