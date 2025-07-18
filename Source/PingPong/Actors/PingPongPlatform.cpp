// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"
#include "PingPong/PlayerStates/PongPlayerState.h"

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
	BallsPoolComponent = CreateDefaultSubobject<UPongBallPool>(TEXT("BallsPoll"));
	BallsPoolComponent->SetIsReplicated(true);
	bReplicates=true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LoadedBodyMeshRoot(TEXT("/Script/Engine.StaticMesh'/ControlRig/Controls/ControlRig_Circle_1mm.ControlRig_Circle_1mm'"));
	
}

// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartRotatePos = BodyMesh->GetComponentRotation().Yaw;
	SetReplicateMovement(true);
	InitialZ=GetActorLocation().Z;
	GameState = Cast<APongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	FVector Origin,BoxExtended;
	GetActorBounds(false,Origin,BoxExtended,false);
	UKismetSystemLibrary::PrintText(GetWorld(),FText::AsNumber(BoxExtended.X));
	currentLocation = MeshRoot->GetComponentLocation();
	MoveSpeedMax=MoveSpeed*MoveSpeedMultiplier;
	MoveSpeedMin=MoveSpeed/MoveSpeedMultiplier;
}


// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Server_MoveRight(DeltaTime);
	Server_MoveForward(DeltaTime);
	FVector vectorSpeed = (currentLocation - MeshRoot->GetComponentLocation());
	double speed = vectorSpeed.X+vectorSpeed.Y;
	speed =  FMath::Abs(speed);
	currentLocation = MeshRoot->GetComponentLocation();
	//TODO FIX SOUND ON CLIENT SIDE
	SetSoundPitch(FMath::Max(FMath::Abs(targetForwardAxisValue),FMath::Abs(targetRightAxisValue))*5);
	//UE_LOG(LogTemp,Warning,TEXT("speed=%f"),FMath::Max(FMath::Abs(targetForwardAxisValue),FMath::Abs(targetRightAxisValue)))
}

bool APingPongPlatform::Server_Fire_Validate(EBallModificators Modificator)
{
	return true;
}

void APingPongPlatform::Server_GetRightValue_Implementation(float AxisValue)
{
	CurrentRightAxisValue=AxisValue;
}

bool APingPongPlatform::Server_GetRightValue_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_GetForwardValue_Implementation(float AxisValue)
{
	CurrentForwardAxisValue=AxisValue;
}

bool APingPongPlatform::Server_GetForwardValue_Validate(float AxisValue)
{
	return true;
}

UPlatformModificator* APingPongPlatform::GetPlatformModificator() const
{
	return PlatformModificator;
}

void APingPongPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APingPongPlatform::SetSpeedMultiplier(float Multiplier)
{	
	MoveSpeed*=Multiplier;
	MoveSpeed= FMath::Clamp(MoveSpeed,MoveSpeedMin,MoveSpeedMax);
}

bool APingPongPlatform::CheckScore(EBallModificators Modificator)
{
	APongPlayerState* PlayerState = Cast<APongPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	check(PlayerState);
	if(PlayerState->GetScore()-GameState->GetShotCost(Modificator)>0)
	{
		PlayerState->SetScore(PlayerState->GetScore()-GameState->GetShotCost(Modificator));
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
	if (!bFloating) return;
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = InitialZ + Amplitude * FMath::Sin(Frequency * RunningTime);
	SetActorLocation(NewLocation);
	RunningTime += 0.01;
}

void APingPongPlatform::Server_Rotate_Implementation(float AxisValue)
{
	if(AxisValue!=0)
	{
		FRotator Rotator;
		Rotator.Yaw = BodyMesh->GetRelativeRotation().Yaw + AxisValue;
		Rotator.Pitch=0;
		Rotator.Roll=0;
		Rotator.Yaw =FMath::Clamp(Rotator.Yaw,-RotateAngle,RotateAngle);
		BodyMesh->SetRelativeRotation(Rotator);
	}
}

bool APingPongPlatform::Server_Rotate_Validate(float AxisValue)
{
	return true;
}

void APingPongPlatform::Server_Fire_Implementation(EBallModificators Modificator)
{
	if(!CheckScore(Modificator)) return; //enough points to shoot
	FTransform Transform;
	Transform.SetLocation(ShootDirectionArrow->GetComponentLocation());
	Transform.SetRotation(ShootDirectionArrow->GetComponentRotation().Quaternion());
	BallsPoolComponent->SpawnBallOnServer(this,GetOwner(),Transform,Modificator);
	//TODO why is it spawning when it should be already exist in pool?
}

void APingPongPlatform::Server_MoveForward_Implementation(float DeltaTime)
{
	if(bInvertedControl)
	{
		CurrentForwardAxisValue=-CurrentForwardAxisValue;
	}	
	targetForwardAxisValue = FMath::Lerp(targetForwardAxisValue,CurrentForwardAxisValue,InterpolationKey);
	SetSoundPitch(FMath::Abs(targetForwardAxisValue)+1);	
	FVector nextLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * targetForwardAxisValue*DeltaTime;
	if(!SetActorLocation(nextLocation, true))
	{
		
	}
}

bool APingPongPlatform::Server_MoveForward_Validate(float DeltaTime)
{
	return true;
}

void APingPongPlatform::Server_MoveRight_Implementation(float DeltaTime)
{
	if(bInvertedControl)
	{
		CurrentRightAxisValue=-CurrentRightAxisValue;
	}	
	targetRightAxisValue = FMath::Lerp(targetRightAxisValue,CurrentRightAxisValue,InterpolationKey);
	
	FVector nextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * targetRightAxisValue*DeltaTime;		
	if(!SetActorLocation(nextLocation, true))
	{
		
	}
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}

