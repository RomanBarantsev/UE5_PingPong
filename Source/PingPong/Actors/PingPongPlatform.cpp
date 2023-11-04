// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"

// Sets default values
APingPongPlatform::APingPongPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	MeshRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBody BodyMeshRoot"));
	SetRootComponent(MeshRoot);
	MeshRoot->SetIsReplicated(true);
	MeshRoot->bHiddenInGame=true;
	MeshRoot->SetWorldScale3D(FVector(3,3,3));
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBody Mesh"));
	BodyMesh->SetupAttachment(RootComponent);	
	BodyMesh->SetIsReplicated(true);
	BodyMesh->SetWorldScale3D(FVector(0.33,1,0.33));
	ShootDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Direction Arrow"));
	ShootDirectionArrow->SetupAttachment(BodyMesh);
	ShootDirectionArrow->SetIsReplicated(true);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetAutoActivate(true);
	PlatformModificator = CreateDefaultSubobject<UPlatformModificator>(TEXT("Platform Modificator"));
	BallsPoolComponent = CreateDefaultSubobject<UPingPongBallPool>(TEXT("BallsPoll"));
	BallsPoolComponent->SetIsReplicated(true);
	bReplicates=true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LoadedBodyMeshRoot(TEXT("/Script/Engine.StaticMesh'/ControlRig/Controls/ControlRig_Circle_1mm.ControlRig_Circle_1mm'"));
	
}

// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	InitialZ=GetActorLocation().Z;
	GameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	FVector Origin,BoxExtended;
	GetActorBounds(false,Origin,BoxExtended,false);
	UKismetSystemLibrary::PrintText(GetWorld(),FText::AsNumber(BoxExtended.X));
}


// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void APingPongPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( APingPongPlatform, AxisMoveValue );
}

void APingPongPlatform::SetSpeedMultiplier(int32 Multiplier)
{
	MoveSpeed=MoveSpeed*Multiplier;
}

bool APingPongPlatform::CheckScore(EModificators Modificator)
{
	APingPongPlayerState* PlayerState = Cast<APingPongPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	check(PlayerState);
	if(PlayerState->GetScore()-GameState->GetModificatorShotCost(Modificator)>0)
	{
		PlayerState->SetScore(PlayerState->GetScore()-GameState->GetModificatorShotCost(Modificator));
		return true;
	}
	return false;
}


void APingPongPlatform::SetSoundPitch_Implementation(float pitch)
{
	AudioComponent->SetPitchMultiplier(pitch);
}

void APingPongPlatform::Floating_Implementation()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = InitialZ + Amplitude * FMath::Sin(Frequency * RunningTime);
	SetActorLocation(NewLocation);
	RunningTime += 0.01;
}


bool APingPongPlatform::Server_Fire_Validate(EModificators Modificator)
{
	return true;
}

void APingPongPlatform::Server_Rotate_Implementation(float AxisValue)
{
	if(AxisValue!=0)
	{
		FRotator Rotator;
		Rotator.Yaw = BodyMesh->GetComponentRotation().Yaw + AxisValue;
		BodyMesh->SetWorldRotation(Rotator);
		//TODO Clamp to angle, and return rotation back on some time.
	}
}

bool APingPongPlatform::Server_Rotate_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_Fire_Implementation(EModificators Modificator)
{
	if(!CheckScore(Modificator)) return;
	APingPongBall* PingPongBall = BallsPoolComponent->GetBall();
	FTransform Transform;
	Transform.SetLocation(ShootDirectionArrow->GetComponentLocation());
	Transform.SetRotation(ShootDirectionArrow->GetComponentRotation().Quaternion());
	BallsPoolComponent->SpawnBallOnServer(GetOwner(),Transform,Modificator);			
}

void APingPongPlatform::Server_MoveForward_Implementation(float AxisValue)
{
	AxisMoveValue = AxisValue;	
	if(AxisValue != 0)
	{
		if(bInvertedControl)
		{
			AxisValue=-AxisValue;
		}
		SetSoundPitch(2);
		FVector currLocation = GetActorLocation();
		FVector nextLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * AxisValue;
		auto lerpNewLocation = UKismetMathLibrary::VLerp(currLocation,nextLocation,1);
		if(!SetActorLocation(lerpNewLocation, true))
		{
			
		}
	}
	else
	{
		SetSoundPitch(1);
	}
}

bool APingPongPlatform::Server_MoveForward_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	AxisMoveValue = AxisValue;
	if(bInvertedControl)
	{
		AxisValue=-AxisValue;
	}
	if(AxisValue != 0)
    {
		SetSoundPitch(2);
	    FVector currLocation = GetActorLocation();
		FVector nextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;
		auto lerpNewLocation = UKismetMathLibrary::VLerp(currLocation,nextLocation,1);
		if(!SetActorLocation(lerpNewLocation, true))
		{
		
		}
    }
	else
	{
		SetSoundPitch(1);
	}
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}

