// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerPawn.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"

// Sets default values
APingPongPlayerPawn::APingPongPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent * root =CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	SetReplicates(true);
	bReplicates=true;
}

// Called every frame
void APingPongPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void APingPongPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


void APingPongPlayerPawn::SetReadyState()
{
	auto playerState = Cast<APingPongPlayerState>(GetPlayerState());
	checkfSlow(playerState,TEXT("PlayerPawn: PlayerState failed to Get"));	
	playerState->SetReady(true);	
}

