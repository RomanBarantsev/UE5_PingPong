// Fill out your copyright notice in the Description page of Project Settings.


#include "PongBallPool.h"

#include <Engine/World.h>
#include <Net/UnrealNetwork.h>

#include "DiffResults.h"
#include "../Actors/PongBall.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPongBallPool::UPongBallPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	
}


// Called when the game starts
void UPongBallPool::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	
}


// Called every frame
void UPongBallPool::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


void UPongBallPool::SpawnBallOnServer_Implementation(AActor* Owner, FTransform spawnTransform,EBallModificators modification)
{
	APongBall* PingPongBall=nullptr;
	for (const auto& Ball : BallsPool)
	{
		if(Ball->IsHidden())
		{
			PingPongBall =  Ball;
		}
	}	
	if(!PingPongBall)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner=Owner;
		PingPongBall = GetWorld()->SpawnActor<APongBall>(BallClass,spawnParams);
		AddBallToPool(PingPongBall);
	}
	PingPongBall->SetModification(modification);
	PingPongBall->SetActorHiddenInGame(false);
	PingPongBall->SetActorLocation(spawnTransform.GetLocation());
	PingPongBall->RotateBallTo(spawnTransform.GetRotation().Rotator());
	PingPongBall->SetActorEnableCollision(true);
	PingPongBall->SetColor();
	PingPongBall->StartMove();
}

bool UPongBallPool::ReleaseBall_Validate(APongBall* PingPongBall)
{
	return true;
}

void UPongBallPool::AddBallToPool_Implementation(APongBall* Ball)
{
	BallsPool.Add(Ball);
}

void UPongBallPool::ReleaseBall_Implementation(APongBall* PingPongBall)
{
	PingPongBall->SetActorEnableCollision(false);
	PingPongBall->SetHidden(true);
	PingPongBall->SetActorRotation(FRotator::ZeroRotator);
	PingPongBall->SetActorLocation(FVector::Zero());
}

void UPongBallPool::FillPool_Implementation()
{
	for(int i=0;i<PoolSize;i++)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner=GetOwner();
		APongBall* PingPongBall = GetWorld()->SpawnActor<APongBall>(BallClass,spawnParams);
		PingPongBall->SetActorHiddenInGame(true);
		PingPongBall->SetActorRotation(FRotator::ZeroRotator);
		PingPongBall->SetActorLocation(FVector::Zero());
		PingPongBall->SetActorEnableCollision(false);
		BallsPool.Add(PingPongBall);		
	}
}
