// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"
#include "PingPongGoal.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/PingPongGameStateBase.h"

// Sets default values
APingPongBall::APingPongBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball Body Collider"));
	SetRootComponent(BodyCollision);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Body	Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);	
	SetReplicateMovement(true);
	bReplicates=true;	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LoadedBallMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
}

// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	Super::BeginPlay();
	StartPosition = GetActorLocation();
	PingPongGameState = Cast<APingPongGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	verify(PingPongGameState);
	BodyCollision->OnComponentBeginOverlap.AddDynamic(this,&APingPongBall::OnCollisionBeginOverlap);
	BodyMesh->OnComponentBeginOverlap.AddDynamic(this,&APingPongBall::OnCollisionBeginOverlap);
	
}

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	if (GetNetMode() != ENetMode::NM_Client)
	{
		Server_Move(DeltaTime);
	}

}

void APingPongBall::StartMove()
{
	Server_StartMove();
}

void APingPongBall::StopMove()
{
	Server_StopMove();
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APingPongBall,isMoving);
}

void APingPongBall::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if(APingPongGoal* PingPongGoal = Cast<APingPongGoal>(OtherActor))
	{
		if(PingPongGoal->Tags[0]=="Blue" && PingPongGameState)
		{
			PingPongGameState->AddScoreToGreenPlayer(PingPongGameState->GetBallHits());
			ResetBall();
		}
		else
		{
			PingPongGameState->AddScoreToBluePlayer(PingPongGameState->GetBallHits());
			ResetBall();
		}
	}
	else
	{
		PingPongGameState->AddValueToBallHits(1);
		UE_LOG(LogTemp, Warning, TEXT("Ball %s Collided with %s"), *GetName(),*SweepResult.GetActor()->GetName());
		FVector moveVector = forward - currLoc;
		moveVector.Normalize();
		DrawDebugDirectionalArrow(GetWorld(), newLoc + moveVector * 300, newLoc,30, FColor::Yellow, true, 3.f, 0, 3);
		FVector impactCorrection = SweepResult.ImpactPoint + SweepResult.ImpactNormal * 300;
		DrawDebugDirectionalArrow(GetWorld(), SweepResult.ImpactPoint,SweepResult.ImpactPoint + SweepResult.ImpactNormal * 300, 30, FColor::Blue, true, 3, 0,3);
		float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(moveVector,SweepResult.ImpactNormal)));
		moveVector = moveVector.RotateAngleAxis(AimAtAngle*2, FVector(0,0,1));
		FVector newTargetMove = newLoc + moveVector * 300;
		newTargetMove.Z = currLoc.Z;
		DrawDebugDirectionalArrow(GetWorld(), newLoc, newTargetMove, 30,FColor::Yellow, true, 3.f, 0, 3);
		FRotator currRotation = GetActorRotation();
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currLoc,newTargetMove);
		newRotation.Pitch = currRotation.Pitch;
		SetActorRotation(newRotation);
		Multicast_HitEffect();
		
	}
}


void APingPongBall::ResetBall()
{
	SetActorLocation(StartPosition);
	PingPongGameState->ResetBallHits();
}

void APingPongBall::Multicast_HitEffect_Implementation()
{
	UWorld * world = GetWorld();
	if(world && HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,GetActorLocation());
	}
}

void APingPongBall::Server_StopMove_Implementation()
{
	isMoving = false;
}

bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}

void APingPongBall::Server_StartMove_Implementation()
{
	isMoving = true;
}

bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}

void APingPongBall::Server_Move_Implementation(float DeltaTime)
{
	// TArray<AActor*> Actors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongPlayerPawn::StaticClass(),Actors);
	// if(Actors.Num()!=2) return;
	
	forward = GetActorForwardVector();
	currLoc = GetActorLocation();
	newLoc = currLoc + forward * MoveSpeed * DeltaTime;
	FHitResult hitResult;
	if(!SetActorLocation(newLoc, true, &hitResult))
	{
		
    }
}

bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}

