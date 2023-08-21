// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPong/ActorComponents/PingPongBallPool.h"
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
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category =	"Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UPingPongBallPool* BallPool;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UArrowComponent* ShootDirectionArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 15;
	float RotationInterpolationKey=0.1f;
	
public:
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveRight(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveForward(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_Rotate(float AxisValue);
	UFUNCTION(Server,Reliable, WithValidation)
	void Server_Fire();

protected:
	UPROPERTY()
	FRotator DefaultRotation;
};
