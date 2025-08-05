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
	ClampReflectionAngel();
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

void AClassinPongBall::ClampReflectionAngel()
{
	FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
	if (Velocity.IsNearlyZero()) return;

	// Only consider X-Y plane for 2D pong
	FVector2D Direction2D = FVector2D(Velocity.X, Velocity.Y).GetSafeNormal();

	// Convert to angle (in degrees)
	float AngleRad = FMath::Atan2(Direction2D.Y, Direction2D.X);
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);

	// Snap to nearest 45 degrees	
	if (AngleDeg<45 || AngleDeg>0) AngleDeg=45.0f;
	else if (AngleDeg>135 || AngleDeg<180) AngleDeg=135.0f;
	else if (AngleDeg>135 || AngleDeg<180) AngleDeg=135.0f;
	else if (AngleDeg<-45 || AngleDeg>0) AngleDeg=-45.0f;
	else if (AngleDeg<-135 || AngleDeg>-180) AngleDeg=-135.0f;
	// Convert back to direction vector
	float SnappedAngleRad = FMath::DegreesToRadians(AngleDeg);
	FVector2D SnappedDirection2D = FVector2D(FMath::Cos(AngleDeg), FMath::Sin(AngleDeg));
	FVector NewVelocity = FVector(SnappedDirection2D.X, SnappedDirection2D.Y, 0) * MoveSpeed;

	BodyMesh->SetPhysicsLinearVelocity(NewVelocity);
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

