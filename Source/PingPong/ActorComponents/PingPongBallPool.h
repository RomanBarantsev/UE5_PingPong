// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PingPong/Actors/PingPongBall.h"
#include "PingPongBallPool.generated.h"


class APingPongBallModificated;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PINGPONG_API UPingPongBallPool : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPingPongBallPool();

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
	TSubclassOf<APingPongBall> BallClass;
	UPROPERTY()
	TArray<APingPongBall*> BallsPool;
public:
	UFUNCTION(Server,Reliable,WithValidation)
	void ReleaseBall(APingPongBall* PingPongBall);
	UFUNCTION()
	APingPongBall* GetBall();
	UFUNCTION()	
	void AddBallToPool(APingPongBall* Ball);
	UFUNCTION(Server,Reliable)
	void SpawnBallOnServer(AActor* Owner, FTransform spawnTransform,EModificators modification);
};

