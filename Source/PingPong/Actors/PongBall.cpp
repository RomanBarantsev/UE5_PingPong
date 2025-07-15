// Fill out your copyright notice in the Description page of Project Settings.


#include "PongBall.h"

#include "BallGCActor.h"
#include "PongGoal.h"
#include "PingPongPlatform.h"
#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PingPong/GameModes/PongGameMode.h"
#include "PingPong/GameStates/PongGameState.h"
#include "PingPong/PlayerStates/PongPlayerState.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionSizeSpecificUtility.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APongBall::APongBall()
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
	BodyMesh->GetBodyInstance()->bLockZTranslation = true;
	BodyMesh->GetBodyInstance()->bLockZRotation = true;
	BodyMesh->GetBodyInstance()->SetDOFLock(EDOFMode::XYPlane);
	SpeedEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("SpeedEffectComponent");
	//SpeedEffectComponent->SetupAttachment(BodyMesh,"NONE");
	SpeedEffectComponent->bAutoActivate = false;
	SpeedEffectComponent->SetRelativeRotation(FRotator(90, 0, 0));
	SpeedEffectComponent->SetRelativeLocation(FVector(0, 0, 30));
	BodyMesh->BodyInstance.bLockZTranslation = true;
	BodyMesh->BodyInstance.bLockXRotation = true;
	BodyMesh->BodyInstance.bLockYRotation = true;
	BodyMesh->BodyInstance.bLockZRotation = true;
	
	// Apply settings
	BodyMesh->RecreatePhysicsState();
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
	BodyMesh->OnComponentHit.AddDynamic(this,&APongBall::OnHit);
	BodyMesh->BodyInstance.bUseCCD = true;
	bReplicates=true;	
}

void APongBall::MatchStateChanged(FName NewState)
{
	if (NewState==MatchState::InProgress)
	{
		double rand = UKismetMathLibrary::RandomFloatInRange(0,90);
		RotateBallTo(FRotator(rand,rand,rand));
		StartMove();
	}
	if (NewState==MatchState::WaitingPostMatch)
	{
		StopMove();
	}
}

// Called when the game starts or when spawned
void APongBall::BeginPlay()
{
	bReplicates=true;
	SetReplicateMovement(true);	
	PingPongGameState = Cast<APongGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PingPongGameMode = Cast<APongGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PingPongGameState->OnMatchStateChanged.AddDynamic(this,&ThisClass::MatchStateChanged);
	Super::BeginPlay();
}

// Called every frame
void APongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APongBall::SetOwningPlaform(APingPongPlatform* Platform)
{
	OwningPlatform = Platform;
}

void APongBall::StartMove()
{
	Server_StartMove();
}

void APongBall::StopMove()
{
	Server_StopMove();
}

void APongBall::RotateBallTo(FRotator Rotator)
{
	SetActorRotation(Rotator);
}

void APongBall::SetBallOwner(FHitResult HitResult)
{
	if(Cast<APingPongPlatform>(HitResult.GetActor()))
	{		
		LastTouchedPlatform=Cast<APingPongPlatform>(HitResult.GetActor());
		SetOwner(LastTouchedPlatform->GetOwner());
	}	
}

void APongBall::CheckGoal_Implementation(FHitResult HitResult)
{
	APongGoal* PingPongGoal = Cast<APongGoal>(HitResult.GetActor());
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

void APongBall::AddScoreToPlayer_Implementation(AActor* Player)
{
	APongPlayerState* PingPongPlayerState=GetOwner()->GetInstigatorController()->GetPlayerState<APongPlayerState>();
	check(PingPongPlayerState);			
	PingPongPlayerState->SetScore(PingPongPlayerState->GetScore()+PingPongGameState->GetModificatorPoints(Modificator));
	PingPongGameState->UpdatePlayersScore(PingPongPlayerState->GetPlayerId(),PingPongPlayerState->GetScore());
	PingPongGameState->AddMaxScore(PingPongPlayerState->GetScore());
}

void APongBall::ReturnToPool()
{
	SpawnChaosBall();
	StopMove();
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void APongBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	if (IsHidden())
		return;
	if(HasAuthority())
	{
		OnBallHitAnything(Hit);
		IncreaseBallSpeed();
	}	
}


void APongBall::SpawnChaosBall_Implementation()
{
	FActorSpawnParameters params;
	params.Owner = this;
	ABallGCActor* SpawnedGeometryActor = GetWorld()->SpawnActor<ABallGCActor>(BallGCActor,GetActorLocation(),GetActorRotation(),params);
	SpawnedGeometryActor->DispatchBeginPlay();
	SpawnedGeometryActor->InitializeComponents();
	SpawnedGeometryActor->PostActorConstruction();
	SpawnedGeometryActor->SetMaterialColor(BallColor);
	SpawnedGeometryActor->SetActorLocation(GetActorLocation());
	SpawnedGeometryActor->GetGeometryCollectionComponent()->Activate(true);
}

void APongBall::PlayHitPlatformSound_Implementation()
{
	HitPlatformSound->Play();
}

void APongBall::PlayHitWallSound_Implementation()
{
	HitWallSound->Play();
}

void APongBall::SetColor_Implementation()
{
	if(!DynamicMaterial)
	{
		auto Material = BodyMesh->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
		BodyMesh->SetMaterial(0,DynamicMaterial);
	}
	DynamicMaterial->SetVectorParameterValue(TEXT("color"),BallColor);
}

void APongBall::OnPlatformHitModificator_Implementation(FHitResult hitResult)
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
				OwningPlatform->GetPlatformModificator()->SetSpeedOfPlatform(PingPongGameState->GetBallModificationValue(EBallModificators::Fast));				
			}
			if(Modificator==EBallModificators::Slow)
			{
				PlatformModificator->SetSpeedOfPlatform(PingPongGameState->GetBallModificationValue(EBallModificators::Slow));
			}
			if(Modificator==EBallModificators::Shrink)
			{
				PlatformModificator->SetPlatformSize(-PingPongGameState->GetBallModificationValue(EBallModificators::Shrink));
			}
			if(Modificator==EBallModificators::Expand)
			{
				OwningPlatform->GetPlatformModificator()->SetPlatformSize(PingPongGameState->GetBallModificationValue(EBallModificators::Expand));
			}
			if(Modificator==EBallModificators::ReverseControl)
			{
				PlatformModificator->SetReverseControl();
			}
			SetModification(EBallModificators::None);	
		}
	}	
}

void APongBall::SetModification_Implementation(EBallModificators mod)
{
	Modificator=mod;	
	BallColor = PingPongGameState->GetModificatorColor(mod);
	MoveSpeed = PingPongGameState->GetBallSpeed(mod);
	SetColor();
}

bool APongBall::SetModification_Validate(EBallModificators mod)
{
	return true;
}

void APongBall::Server_StartMove()
{
	isMoving = true;
	MoveSpeed=MinBallSpeed;
	FVector Impulse;
	Impulse = FVector(BodyMesh->GetForwardVector());
	BodyMesh->AddImpulse(Impulse);	
	IncreaseBallSpeed();
}

void APongBall::IncreaseBallSpeed()
{
	if(MoveSpeed<MaxBallSpeed)
	{
		MoveSpeed+=IncreaseSpeedStep;
		FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
		BodyMesh->SetPhysicsLinearVelocity(UKismetMathLibrary::ClampVectorSize(Velocity,MoveSpeed,MoveSpeed));
		Multicast_SpeedEffect(false);
	}
	else
	{
		Multicast_SpeedEffect(true);
	}
		
}

void APongBall::Multicast_SpeedEffect_Implementation(bool Enable)
{
	SpeedEffectComponent->SetActive(Enable);
}

void APongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APongBall,isMoving);
	DOREPLIFETIME(APongBall,BallColor);
}

void APongBall::OnBallHitAnything_Implementation(FHitResult hitResult)
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

void APongBall::Multicast_HitEffect_Implementation(FVector location)
{
	UWorld * world = GetWorld();
	if(world && HitEffect)
	{		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,location);		
	}
}

void APongBall::Server_StopMove_Implementation()
{
	BodyMesh->SetPhysicsLinearVelocity(FVector::Zero());
	isMoving = false;
}

bool APongBall::Server_StopMove_Validate()
{
	return true;
}
