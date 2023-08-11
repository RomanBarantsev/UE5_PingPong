// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"
#include "PingPongGoal.h"
#include "PingPongPlatform.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "PingPong/GameModes/PingPongGameMode.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"

// Sets default values
APingPongBall::APingPongBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Body	Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	BodyMesh->SetWorldScale3D(FVector3d(0.3,0.3,0.3));	
	bReplicates=true;	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LoadedBallMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
}

// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	SetReplicateMovement(true);	
	StartPosition = GetActorLocation();	
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PingPongGameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	BodyMesh->OnComponentBeginOverlap.AddDynamic(this,&APingPongBall::OnCollisionBeginOverlap);
	if(PingPongGameMode)
	{
//		PingPongGameMode->OnMatchStateChanged.AddUObject(this,&APingPongBall::BallMatchState);
	}
	ResetBall();
	Super::BeginPlay();
}

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	if (GetNetMode() != ENetMode::NM_Client && isMoving)
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

void APingPongBall::OnBallHitAnything(FHitResult hitResult)
{
	if(APingPongGoal* PingPongGoal = Cast<APingPongGoal>(hitResult.GetActor()))
	{
		AActor* owner = PingPongGoal->GetOwner();
		APingPongPlayerState* PingPongPlayerState =  owner->GetInstigatorController()->GetPlayerState<APingPongPlayerState>();
		check(PingPongPlayerState);
		PingPongPlayerState->SetScore(PingPongPlayerState->GetScore()+1);
	}
}

void APingPongBall::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if(APingPongGoal* PingPongGoal = Cast<APingPongGoal>(OtherActor))
	{
		
	}	
}

void APingPongBall::BallMatchState(FName matchState)
{
	if(matchState==MatchState::InProgress)
		isMoving=true;
}

void APingPongBall::ResetBall()
{	
	SetActorLocation(StartPosition);	
	RotateBallToRotator();
}

void APingPongBall::RotateBallToRotator_Implementation(FRotator Rotator)
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongPlatform::StaticClass(),Actors);	
	if(!Actors.IsEmpty() && Rotator==FRotator::ZeroRotator)
	{		
		FVector Direction = GetActorLocation()-Actors[(UKismetMathLibrary::RandomInteger(Actors.Num()-1))]->GetActorLocation();
		Direction.Normalize();
		FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(FRotator(0,TargetRotation.Yaw-UKismetMathLibrary::RandomInteger(45),0));
	}
	else
	{
		SetActorRotation(Rotator);
	}
	FQuat ActorQuat = GetActorRotation().Quaternion();
	forwardVector=ActorQuat.GetForwardVector();
}

void APingPongBall::SetIsMoving(bool value,FRotator Rotator)
{
	isMoving=value;
	RotateBallToRotator();
}

void APingPongBall::Multicast_HitEffect_Implementation(FVector location)
{
	UWorld * world = GetWorld();
	if(world && HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,location);
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
	currLoc = GetActorLocation();
	newLoc = currLoc + forwardVector * MoveSpeed * DeltaTime;
	FHitResult hitResult;
	if(!SetActorLocation(newLoc, true, &hitResult))
	{
		FVector Vec = UKismetMathLibrary::MirrorVectorByNormal(hitResult.TraceEnd-hitResult.TraceStart,hitResult.ImpactNormal);
		Vec.Normalize();
		forwardVector=FVector(Vec.X,Vec.Y,0);
		Multicast_HitEffect(hitResult.Location);
		OnBallHitAnything(hitResult);
    }
}

bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}

