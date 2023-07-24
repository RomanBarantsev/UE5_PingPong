// Fill out your copyright notice in the Description page of Project Settings.


#include "BallsPool.h"

#include "Kismet/GameplayStatics.h"

UBallsPool::UBallsPool()
{
	
}

void UBallsPool::FillPool()
{
	UWorld* World = GEngine->GameViewport->GetWorld();
	for (int i=0;i<MaxPoolSize;i++)
	{
		if (World)
		{
			FRotator rotation = FRotator::ZeroRotator;
			FVector location = FVector::Zero();
			APingPongBall* PingPongBall = World->SpawnActor<APingPongBall>(location,rotation);
			PingPongBall->SetHidden(true);
			PingPongBall->SetActorEnableCollision(false);
			PingPongBalls.Add(PingPongBall);
		}
		
	}
}
