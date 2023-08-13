// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGoal.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Pawns/PingPongPlayerPawn.h"

// Sets default values
APingPongGoal::APingPongGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(RootComponent);	
	bReplicates=true;
	
}

// Called when the game starts or when spawned
void APingPongGoal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APingPongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


