// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBallPool.h"

#include <Engine/World.h>
#include <Net/UnrealNetwork.h>

#include "../Actors/PingPongBall.h"


// Sets default values for this component's properties
UPingPongBallPool::UPingPongBallPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UPingPongBallPool::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	FillPool();
}


// Called every frame
void UPingPongBallPool::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPingPongBallPool::FillPool_Implementation()
{
	for(int i=0;i<PoolSize;i++)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner=GetOwner();
		APingPongBall* PingPongBall = GetWorld()->SpawnActor<APingPongBall>(BallClass,spawnParams);
		PingPongBall->SetActorHiddenInGame(true);
		PingPongBall->SetActorRotation(FRotator::ZeroRotator);
		PingPongBall->SetActorLocation(FVector::Zero());
		PingPongBall->SetActorEnableCollision(false);
		BallsPool.Add(PingPongBall);
		
	}
}

void UPingPongBallPool::ReleaseBall(APingPongBall* PingPongBall)
{
	PingPongBall->SetHidden(true);
	PingPongBall->SetActorRotation(FRotator::ZeroRotator);
	PingPongBall->SetActorLocation(FVector::Zero());
	
}

APingPongBall* UPingPongBallPool::GetBall()
{
	for (auto Ball : BallsPool)
	{
		if(Ball->IsHidden())
		{
			return Ball;
		}
	}	
	return nullptr;
}

