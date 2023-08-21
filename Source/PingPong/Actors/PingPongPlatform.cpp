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
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
	SceneComponent->SetIsReplicated(true);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBody Mesh"));
	BodyMesh->SetupAttachment(SceneComponent);	
	BodyMesh->SetIsReplicated(true);
	ShootDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Direction Arrow"));
	ShootDirectionArrow->SetupAttachment(BodyMesh);
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
	DefaultRotation = BodyMesh->GetRelativeRotation();
}


// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

bool APingPongPlatform::Server_Fire_Validate()
{
	return true;
}

void APingPongPlatform::Server_Rotate_Implementation(float AxisValue)
{
	if(AxisValue!=0)
	{
		FRotator Rotator;
		//if(BodyMesh->GetComponentRotation().Yaw>DefaultRotation.Yaw+90 || BodyMesh->GetComponentRotation().Yaw<DefaultRotation.Yaw-90) return;
		Rotator.Yaw = BodyMesh->GetComponentRotation().Yaw + AxisValue;
		BodyMesh->SetWorldRotation(Rotator);
		//TODO Clamp to angle, and return rotation back on some time.
	}
}

bool APingPongPlatform::Server_Rotate_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_Fire_Implementation()
{
	APingPongBall* Ball = BallPool->GetBall();
	Ball->SetActorHiddenInGame(false);
	Ball->SetActorLocation(ShootDirectionArrow->GetComponentLocation());
	Ball->RotateBallTo(ShootDirectionArrow->GetComponentRotation());
	Ball->SetActorEnableCollision(true);
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

