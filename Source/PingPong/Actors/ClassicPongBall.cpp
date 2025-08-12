// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongBall.h"

#include "PongGoal.h"
#include "Components/AudioComponent.h"
#include "PingPong/PlayerStates/PongPlayerState.h"


void AClassicPongBall::OnBallHitAnything(FHitResult hitResult)
{
	ClampReflectionAngel();
	APongGoal* PingPongGoal = Cast<APongGoal>(hitResult.GetActor());
	if(PingPongGoal)
	{
		MoveSpeed=MinBallSpeed;
		AddScoreToPlayer(PingPongGoal->GetOwner());
		//Multicast_HitEffect(hitResult.Location);
		return;
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

void AClassicPongBall::AddScoreToPlayer(AActor* Player)
{
	APongPlayerState* PingPongPlayerState=Player->GetInstigatorController()->GetPlayerState<APongPlayerState>();
	check(PingPongPlayerState);
	PingPongPlayerState->SetScore(PingPongPlayerState->GetScore()+1);
	PingPongGameState->UpdatePlayersScore(PingPongPlayerState->GetPlayerId(),PingPongPlayerState->GetScore());
	PingPongGameState->AddMaxScore(PingPongPlayerState->GetScore());
	SetActorLocation((FVector::Zero()));
	PlayGoalSound();
	//Super::AddScoreToPlayer(Player);
}

void AClassicPongBall::ClampReflectionAngel()
{
	FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
	if (Velocity.IsNearlyZero()) return;

	FVector2D Dir2D = FVector2D(Velocity.X, Velocity.Y).GetSafeNormal();

	// Get angle from Y-axis (we use Atan2 with X first because Y is now forward)
	float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Dir2D.X, Dir2D.Y));

	if (Velocity.Y >= 0)
	{
		// Moving UP → clamp to [-45°, +45°] from Y axis
		AngleDeg = FMath::Clamp(AngleDeg, -45.f, 45.f);
	}
	else
	{
		// Moving DOWN → clamp to [135°, 180°] or [-180°, -135°]
		if (AngleDeg > 0)
			AngleDeg = FMath::Clamp(AngleDeg, 135.f, 180.f);
		else
			AngleDeg = FMath::Clamp(AngleDeg, -180.f, -135.f);
	}

	// Convert clamped angle to direction vector
	float ClampedRad = FMath::DegreesToRadians(AngleDeg);
	FVector2D NewDir2D = FVector2D(FMath::Sin(ClampedRad), FMath::Cos(ClampedRad));
	FVector NewVelocity = FVector(NewDir2D.X, NewDir2D.Y, 0.f) * MoveSpeed;

	BodyMesh->SetPhysicsLinearVelocity(NewVelocity);
}

// Sets default values
AClassicPongBall::AClassicPongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GoalSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Goal Sound"));
	GoalSound->SetAutoActivate(false);
	MinBallSpeed = 5000.0f;
}

void AClassicPongBall::PlayGoalSound_Implementation()
{
	GoalSound->Play();
}

// Called when the game starts or when spawned
void AClassicPongBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClassicPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

