// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PingPong/Actors/PongBall.h"
#include "PongBallPool.generated.h"


class APingPongBallModificated;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PINGPONG_API UPongBallPool : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPongBallPool();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
private:
	UFUNCTION(Server,Reliable)	
	void FillPool();	
	UPROPERTY(EditAnywhere)
	int32 PoolSize = 5;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APongBall> BallClass;
	UPROPERTY()
	TArray<APongBall*> BallsPool;
public:
	UFUNCTION(Server,Reliable,WithValidation)
	void ReleaseBall(APongBall* PingPongBall);
	UFUNCTION(Server,Reliable)	
	void AddBallToPool(APongBall* Ball);
	UFUNCTION(Server,Reliable)
	void SpawnBallOnServer(AActor* Owner, FTransform spawnTransform,EBallModificators modification);
};

