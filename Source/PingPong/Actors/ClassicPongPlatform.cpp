// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongPlatform.h"


// Sets default values
AClassicPongPlatform::AClassicPongPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InterpolationKey=1.0f;
	MoveSpeed = 2000.0f;
	bFloating = false;
}

// Called when the game starts or when spawned
void AClassicPongPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClassicPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

