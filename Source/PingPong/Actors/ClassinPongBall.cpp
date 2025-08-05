// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassinPongBall.h"

#include "PongGoal.h"
#include "PingPong/PlayerStates/PongPlayerState.h"


void AClassinPongBall::OnBallHitAnything(FHitResult hitResult)
{
	APongGoal* PingPongGoal = Cast<APongGoal>(hitResult.GetActor());
	if(PingPongGoal)
	{
		MoveSpeed=MinBallSpeed;
		AddScoreToPlayer(PingPongGoal->GetOwner());
		Multicast_HitEffect(hitResult.Location);
	}	
	if(!hitResult.GetActor()->GetOwner())
	{
		Super::PlayHitWallSound();
	}		
	else
	{
		Super::PlayHitPlatformSound();
	}	
	//Super::OnBallHitAnything(hitResult);
}

void AClassinPongBall::AddScoreToPlayer(AActor* Player)
{
	APongPlayerState* PingPongPlayerState=Player->GetInstigatorController()->GetPlayerState<APongPlayerState>();
	check(PingPongPlayerState);
	PingPongPlayerState->SetScore(PingPongPlayerState->GetScore()+1);
	PingPongGameState->UpdatePlayersScore(PingPongPlayerState->GetPlayerId(),PingPongPlayerState->GetScore());
	PingPongGameState->AddMaxScore(PingPongPlayerState->GetScore());
	SetActorLocation((FVector::Zero()));
	//Super::AddScoreToPlayer(Player);
}

// Sets default values
AClassinPongBall::AClassinPongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClassinPongBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClassinPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

