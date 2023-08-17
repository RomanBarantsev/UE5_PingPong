// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

// Sets default values
APingPongPlatform::APingPongPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBody Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	SetRootComponent(BodyMesh);
	BodyMesh->SetIsReplicated(true);
	ShootDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Direction Arrow"));
	ShootDirectionArrow->SetupAttachment(RootComponent);
	ShootDirectionArrow->SetIsReplicated(true);
	BallPool = CreateDefaultSubobject<UPingPongBallPool>("BallsPool");
	BallPool->SetIsReplicated(true);
	bReplicates=true;	
}

// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);		
}


// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APingPongPlayerController* MyPC = Cast<APingPongPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (MyPC)
	{
		FVector WorldLocation;
		FVector WorldDirection;
		FPlane Plane = FPlane(0.f, 0.f, 1.f, 0.f);
		float t;
		FVector IntersectionPoint;
		FVector ActorLocation = ShootDirectionArrow->GetComponentLocation();
		MyPC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		WorldDirection = WorldDirection * 100000;

		if (GetLocalRole() < ROLE_Authority)
		{
			if (UKismetMathLibrary::LinePlaneIntersection(WorldLocation, WorldLocation + WorldDirection, Plane, t, IntersectionPoint))
			{
				FVector dir = (IntersectionPoint - ActorLocation).GetSafeNormal();
				DirectionFaced = FVector(dir.X, dir.Y, 0.f);
				FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation, IntersectionPoint);
				FRotator NewRot = FRotator(0.f, LookAtRot.Yaw, 0.f);
				ShootDirectionArrow->SetWorldRotation(NewRot); //SetActorRotation();
			}

			Server_CalculateRotation(WorldLocation, WorldDirection, ActorLocation);
		}
		else
			if ((GetLocalRole() == ROLE_Authority) && MyPC->IsLocalController()) 
		{
			// Listen Server, treat it as a client but without the RPC call. 
			if (UKismetMathLibrary::LinePlaneIntersection(WorldLocation, WorldLocation + WorldDirection, Plane, t, IntersectionPoint))
			{
				FVector dir = (IntersectionPoint - ActorLocation).GetSafeNormal();
				DirectionFaced = FVector(dir.X, dir.Y, 0.f);
				FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation, IntersectionPoint);
				FRotator NewRot = FRotator(0.f, LookAtRot.Yaw, 0.f);
				ShootDirectionArrow->SetWorldRotation(NewRot); //SetActorRotation(NewRot);
			}
		}
	}
}

void APingPongPlatform::OnRep_SetServerRotation()
{
	ShootDirectionArrow->SetWorldRotation(R_ServerRotation); //SetActorRotation(R_ServerRotation);
}

void APingPongPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(APingPongPlatform, R_ServerRotation);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	
}

void APingPongPlatform::Server_CalculateRotation_Implementation(FVector WorldLocation, FVector WorldDirection,
                                                                FVector ActorLocation)
{
	ShootDirectionArrow->SetWorldRotation(R_ServerRotation); //SetActorRotation(R_ServerRotation);
	FPlane Plane = FPlane(0.f, 0.f, 1.f, 0.f);
	float t;
	FVector IntersectionPoint;

	if (UKismetMathLibrary::LinePlaneIntersection(WorldLocation, WorldLocation + WorldDirection, Plane, t, IntersectionPoint))
	{
		FVector dir = (IntersectionPoint - ActorLocation).GetSafeNormal();
		DirectionFaced = FVector(dir.X, dir.Y, 0.f);
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation, IntersectionPoint);
		FRotator NewRot = FRotator(0.f, LookAtRot.Yaw, 0.f);
		R_ServerRotation = NewRot;
		OnRep_SetServerRotation();
	}
}

bool APingPongPlatform::Server_CalculateRotation_Validate(FVector WorldLocation, FVector WorldDirection,
	FVector ActorLocation)
{
	return true;
}

void APingPongPlatform::Server_Fire_Implementation()
{
	APingPongBall* Ball = BallPool->GetBall();
	Ball->SetActorHiddenInGame(false);
	Ball->SetActorLocation(ShootDirectionArrow->GetComponentLocation());
	Ball->SetActorRotation(ShootDirectionArrow->GetComponentRotation());
	Ball->SetActorEnableCollision(true);
	Ball->RotateBallTo(FRotator(0,90,0));
	Ball->StartMove();
}

void APingPongPlatform::Server_MoveForward_Implementation(float AxisValue)
{
	if(AxisValue != 0)
	{
		FVector currLocation = GetActorLocation();
		FVector nextLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * AxisValue;
		auto lerpNewLocation = UKismetMathLibrary::VLerp(currLocation,nextLocation,1);
		if(!SetActorLocation(lerpNewLocation, true))
		{
			
		}
	}
}

bool APingPongPlatform::Server_MoveForward_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	if(AxisValue != 0)
    {
	    FVector currLocation = GetActorLocation();
		FVector nextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;
		auto lerpNewLocation = UKismetMathLibrary::VLerp(currLocation,nextLocation,1);
		if(!SetActorLocation(lerpNewLocation, true))
		{
		
		}
    }
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}

