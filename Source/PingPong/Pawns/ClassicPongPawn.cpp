// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AClassicPongPawn::AClassicPongPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm->TargetArmLength=0.0f;
	Camera->SetupAttachment(nullptr);
	Camera->SetWorldLocation(FVector(0,0,2200));	
	Camera->SetWorldRotation(FRotator(-90,0,0));
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth=12000;
	Camera->OrthoFarClipPlane=true;
	Camera->SetOrthoFarClipPlane(200000.0);
}

// Called when the game starts or when spawned
void AClassicPongPawn::BeginPlay()
{
	Super::BeginPlay();
	FRotator InitialRotation = GetActorRotation();
	Camera->SetWorldRotation(FRotator(-90,InitialRotation.Yaw-90,0));
}

// Called every frame
void AClassicPongPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AClassicPongPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

