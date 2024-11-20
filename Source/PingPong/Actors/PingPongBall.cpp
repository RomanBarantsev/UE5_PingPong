// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"

#include "BallGCActor.h"
#include "PingPongGoal.h"
#include "PingPongPlatform.h"
#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PingPong/GameModes/PingPongGameMode.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionSizeSpecificUtility.h"

// Sets default values
APingPongBall::APingPongBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Body	Mesh"));
	SetRootComponent(BodyMesh);
	BodyMesh->SetCollisionObjectType(ECC_WorldDynamic);
	BodyMesh->SetIsReplicated(true);
	BodyMesh->SetWorldScale3D(FVector3d(0.3,0.3,0.3));
	BodyMesh->SetSimulatePhysics(true);
	BodyMesh->BodyInstance.SetMassOverride(0);
	BodyMesh->BodyInstance.bLockZTranslation=true;
	BodyMesh->SetNotifyRigidBodyCollision(true);
	// static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> BodyMeshPhysicalAsset(TEXT("/Game/PingPong/Blueprints/NoFrictionMaterial.NoFrictionMaterial"));
	// if(BodyMeshPhysicalAsset.Succeeded())
	// {
	// 	BodyMesh->SetPhysMaterialOverride(BodyMeshPhysicalAsset.Object);
	// }
	//TODO Need to Fix
	HitWallSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Wall Sound"));
	HitWallSound->SetAutoActivate(false);
	HitPlatformSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Platform Sound"));
	HitPlatformSound->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if(MeshAsset.Succeeded())
	{
		BodyMesh->SetStaticMesh(MeshAsset.Object);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,FString{"No MeshAsset is found for ball"});
	}
	BodyMesh->OnComponentHit.AddDynamic(this,&APingPongBall::OnHit);
	bReplicates=true;	
}

// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	SetReplicateMovement(true);	
	PingPongGameState = Cast<APingPongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PingPongGameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(GetWorld()));	
	Super::BeginPlay();
}

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APingPongBall::StartMove()
{
	Server_StartMove();
}

void APingPongBall::StopMove()
{
	Server_StopMove();
}

void APingPongBall::SetBallOwner(FHitResult HitResult)
{
	if(Cast<APingPongPlatform>(HitResult.GetActor()))
	{		
		LastTouchedPlatform=Cast<APingPongPlatform>(HitResult.GetActor());
		SetOwner(LastTouchedPlatform->GetOwner());
	}	
}

void APingPongBall::CheckGoal_Implementation(FHitResult HitResult)
{
	APingPongGoal* PingPongGoal = Cast<APingPongGoal>(HitResult.GetActor());
	if(PingPongGoal)
	{		
		AActor* GoalOwner = PingPongGoal->GetOwner();
		check(GoalOwner);
		if(GoalOwner!=GetOwner())
		{
			PingPongGoal->LightUpLightBox();
			AddScoreToPlayer(GoalOwner);
		}
		Multicast_HitEffect(HitResult.Location);
		if (!Indestructible)
		{
			ReturnToPool();
		}
	}
}

void APingPongBall::AddScoreToPlayer_Implementation(AActor* Player)
{
	APingPongPlayerState* PingPongPlayerState=GetOwner()->GetInstigatorController()->GetPlayerState<APingPongPlayerState>();
	check(PingPongPlayerState);			
	PingPongPlayerState->SetScore(PingPongPlayerState->GetScore()+PingPongGameState->GetModificatorPoints(Modificator));
	PingPongGameState->UpdatePlayersScore(PingPongPlayerState->GetPlayerId(),PingPongPlayerState->GetScore());
}

void APingPongBall::SetSpeed(float Speed)
{
	MoveSpeed=Speed;
}

void APingPongBall::ReturnToPool()
{
	SpawnChaosBall();
	StopMove();
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void APingPongBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	if(HasAuthority())
	{
		OnBallHitAnything(Hit);
		IncreaseBallSpeed();
	}	
}


void APingPongBall::SpawnChaosBall_Implementation()
{
	FActorSpawnParameters params;
	ABallGCActor* SpawnedGeometryActor = GetWorld()->SpawnActor<ABallGCActor>(BallGCActor,GetActorLocation(),GetActorRotation(),params);
	SpawnedGeometryActor->DispatchBeginPlay();
	SpawnedGeometryActor->InitializeComponents();
	SpawnedGeometryActor->PostActorConstruction();
	SpawnedGeometryActor->SetMaterialColor(BallColor);
	SpawnedGeometryActor->SetActorLocation(GetActorLocation());
	SpawnedGeometryActor->GetGeometryCollectionComponent()->Activate(true);	
}

void APingPongBall::PlayHitPlatformSound_Implementation()
{
	HitPlatformSound->Play();
}

void APingPongBall::PlayHitWallSound_Implementation()
{
	HitWallSound->Play();
}

void APingPongBall::SetColor_Implementation()
{
	if(!DynamicMaterial)
	{
		auto Material = BodyMesh->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
		BodyMesh->SetMaterial(0,DynamicMaterial);
	}
	DynamicMaterial->SetVectorParameterValue(TEXT("color"),BallColor);
}

void APingPongBall::OnPlatformHitModificator_Implementation(FHitResult hitResult)
{
	if(APingPongPlatform* PingPongPlatform = Cast<APingPongPlatform>(hitResult.GetActor()))
	{
		if(hitResult.GetActor()->GetOwner())
			PlayHitPlatformSound();
		UActorComponent* ActorComponent = PingPongPlatform->GetComponentByClass(UPlatformModificator::StaticClass());
		if(!ActorComponent)
			return;
		UPlatformModificator* PlatformModificator = Cast<UPlatformModificator>(ActorComponent);
		if(PlatformModificator)
		{
			if(Modificator==EBallModificators::Fast)
			{
				PlatformModificator->SetSpeedOfPlatform(PingPongGameState->GetBallModificationValue(EBallModificators::Fast));
			}
			if(Modificator==EBallModificators::Slow)
			{
				PlatformModificator->SetSpeedOfPlatform(PingPongGameState->GetBallModificationValue(EBallModificators::Slow));
			}
			if(Modificator==EBallModificators::Shrink)
			{
				PlatformModificator->SetPlatformSize();
			}
			if(Modificator==EBallModificators::Expand)
			{
				PlatformModificator->SetPlatformSize();
			}
			if(Modificator==EBallModificators::ReverseControl)
			{
				PlatformModificator->SetReverseControl();
			}
			if(Modificator==EBallModificators::LightsOff)
			{
				
			}
			SetModification(EBallModificators::None);	
		}
	}	
}

void APingPongBall::SetModification_Implementation(EBallModificators mod)
{
	Modificator=mod;	
	BallColor = PingPongGameState->GetModificatorColor(mod);
	MoveSpeed = PingPongGameState->GetBallSpeed(mod);
	SetColor();
}

bool APingPongBall::SetModification_Validate(EBallModificators mod)
{
	return true;
}

void APingPongBall::IncreaseBallSpeed_Implementation()
{
	if(MoveSpeed<MaxBallSpeed)
		MoveSpeed+=IncreaseSpeedStep;
	FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
	BodyMesh->SetPhysicsLinearVelocity(UKismetMathLibrary::ClampVectorSize(Velocity,MoveSpeed,MoveSpeed));
}

bool APingPongBall::IncreaseBallSpeed_Validate()
{
	return true;
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APingPongBall,isMoving);
	DOREPLIFETIME(APingPongBall,BallColor);
}

void APingPongBall::OnBallHitAnything_Implementation(FHitResult hitResult)
{	
	SetBallOwner(hitResult);
	if(!hitResult.GetActor()->GetOwner())
		PlayHitWallSound();
	if(GetOwner())
	{
		CheckGoal(hitResult);
		OnPlatformHitModificator(hitResult);
	}
}

void APingPongBall::RotateBallTo_Implementation(FRotator Rotator)
{
	SetActorRotation(Rotator);
}
void APingPongBall::Multicast_HitEffect_Implementation(FVector location)
{
	UWorld * world = GetWorld();
	if(world && HitEffect)
	{		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,location);		
	}
}

void APingPongBall::Server_StopMove_Implementation()
{
	BodyMesh->SetPhysicsLinearVelocity(FVector::Zero());
	isMoving = false;
}

bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}

void APingPongBall::Server_StartMove_Implementation()
{
	isMoving = true;
	MoveSpeed=MinBallSpeed;
	FVector Impulse;
	Impulse = FVector(BodyMesh->GetForwardVector());
	BodyMesh->AddImpulse(Impulse);	
	IncreaseBallSpeed();
}

bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}
