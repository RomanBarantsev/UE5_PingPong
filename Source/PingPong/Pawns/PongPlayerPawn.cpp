// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerPawn.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PlayerStates/PongPlayerState.h"

// Sets default values
APongPlayerPawn::APongPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent * root =CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(0.0f, -25.0f, 0.0f));	
	SpringArm->TargetArmLength = 1500.0f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(120.f);
	SetReplicates(true);
	bReplicates=true;
}

// Called every frame
void APongPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void APongPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}