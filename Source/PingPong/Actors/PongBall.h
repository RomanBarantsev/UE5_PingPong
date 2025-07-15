// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/Facades/CollectionBoundsFacade.h"
#include "PingPong/GameStates/PongGameState.h"
#include "PongBall.generated.h"

class ABallGCActor;
class AGeometryCollectionActor;
class UGeometryCollection;
class UGeometryCollectionComponent;
class USoundCue;
class APingPongPlatform;
class APongGameMode;
class APongGameState;
class USphereComponent;

UCLASS(Abstract)
class PINGPONG_API APongBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongBall();

protected:
	UFUNCTION()
	void MatchStateChanged(FName NewState);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UAudioComponent* HitWallSound;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UAudioComponent* HitPlatformSound;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MinBallSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MaxBallSpeed = 1000;
	float MoveSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float IncreaseSpeedStep = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystemComponent* SpeedEffectComponent;
	UPROPERTY(Replicated,EditAnywhere)
	bool isMoving = false;
	
protected:
	UFUNCTION()
	void Server_StartMove();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();
	UFUNCTION()
	void IncreaseBallSpeed();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect(FVector location);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpeedEffect(bool Enable);
	virtual void GetLifetimeReplicatedProps(TArray < class FLifetimeProperty >& OutLifetimeProps) const override;
	UFUNCTION(Server,Reliable)
	virtual void OnBallHitAnything(FHitResult hitResult);
	UPROPERTY()
	APingPongPlatform* LastTouchedPlatform;
	UPROPERTY()
	APingPongPlatform* OwningPlatform;
public:
	UFUNCTION()
	void SetOwningPlaform(APingPongPlatform* Platform);
	UFUNCTION(BlueprintCallable)
	void StartMove();
	UFUNCTION(BlueprintCallable)
	void StopMove();
	
public:
	UFUNCTION()
	void RotateBallTo(FRotator Rotator=FRotator::ZeroRotator);
protected:
	UPROPERTY()
	APongGameState* PingPongGameState;
UPROPERTY()
	APongGameMode* PingPongGameMode;
	UPROPERTY()
	TArray<AActor*> PlatformActors;

protected:
	UFUNCTION()
	void SetBallOwner(FHitResult HitResult);
	UFUNCTION(Server,Reliable)
	virtual void AddScoreToPlayer(AActor* Player);
	UFUNCTION(Server,Reliable)
	void CheckGoal(FHitResult HitResult);
	
private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY(ReplicatedUsing="SetColor")
	FLinearColor BallColor;
	UPROPERTY()
	EBallModificators Modificator = EBallModificators::None;	
	UFUNCTION(Server,Reliable)
	void OnPlatformHitModificator(FHitResult hitResult);
public:
	UFUNCTION(Server,Reliable,WithValidation)
	void SetModification(EBallModificators mod);
private:
	UPROPERTY(EditAnywhere);
	bool Indestructible;
	UFUNCTION(Blueprintable,BlueprintCallable)
	void ReturnToPool();
public:
	UFUNCTION(NetMulticast,Reliable)
	void SetColor();
private:
	UFUNCTION(NetMulticast,Reliable)
	void PlayHitWallSound();
	UFUNCTION(NetMulticast,Reliable)
	void PlayHitPlatformSound();
	UFUNCTION(Server,Reliable)
	void SpawnChaosBall();
		
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<ABallGCActor> BallGCActor;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const
	           FHitResult& Hit);
};

