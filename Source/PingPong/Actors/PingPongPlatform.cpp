// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APingPongPlatform::APingPongPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBody Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);	
	SetRootComponent(BodyMesh);
	bReplicates=true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	if(AxisValue != 0)
    {
	    UE_LOG(LogTemp, Warning,TEXT("APingPongPlatform::Server_MoveRight_Implementation"));
    }
    FVector currLocation = GetActorLocation();
    FVector nextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;
    auto lerpNewLocation = UKismetMathLibrary::VLerp(currLocation,nextLocation,1);
    if(!SetActorLocation(lerpNewLocation, true))
    {
		
    }

}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}

