﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PongSpectatorPawn.h"


// Sets default values
APongSpectatorPawn::APongSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APongSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APongSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APongSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

