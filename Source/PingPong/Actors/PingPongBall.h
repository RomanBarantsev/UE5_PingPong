// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/Facades/CollectionBoundsFacade.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPongBall.generated.h"

class ABallGCActor;
class AGeometryCollectionActor;
class UGeometryCollection;
class UGeometryCollectionComponent;
class USoundCue;
class APingPongPlatform;
class APingPongGameMode;
class APingPongGameState;
class USphereComponent;

UCLASS(Abstract)
class PINGPONG_API APingPongBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APingPongBall();

protected:
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
	float MoveSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystem* HitEffect;
	UPROPERTY(Replicated,EditAnywhere)
	bool isMoving = false;	
	FVector forwardVector;
	FVector currLoc;
	FVector newLoc ;
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float DeltaTime);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartMove();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect(FVector location);
	virtual void GetLifetimeReplicatedProps(TArray < class FLifetimeProperty >& OutLifetimeProps) const override;
	UFUNCTION(Server,Reliable)
	virtual void OnBallHitAnything(FHitResult hitResult);
	UPROPERTY()
	APingPongPlatform* LastTouchedPlatform;
public:
	UFUNCTION(BlueprintCallable)
	void StartMove();
	UFUNCTION(BlueprintCallable)
	void StopMove();
	
public:
	UFUNCTION(Server, Reliable)
	void RotateBallTo(FRotator Rotator=FRotator::ZeroRotator);
		
protected:
	UPROPERTY()
	APingPongGameState* PingPongGameState;
UPROPERTY()
	APingPongGameMode* PingPongGameMode;
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
	EModificators Modificator = EModificators::None;	
	UFUNCTION(Server,Reliable)
	void OnPlatformHitModificator(FHitResult hitResult);
public:
	UFUNCTION(Server,Reliable,WithValidation)
	void SetModification(EModificators mod);
	UFUNCTION()
	void SetSpeed(float Speed);
private:
	UFUNCTION(Blueprintable,BlueprintCallable)
	void ReturnToPool();
	UFUNCTION(NetMulticast,Reliable)
	void SetColor();
	UFUNCTION(NetMulticast,Reliable)
	void PlayHitWallSound();
	UFUNCTION(NetMulticast,Reliable)
	void PlayHitPlatformSound();
	UFUNCTION(Server,Reliable)
	void SpawnChaosBall();
	
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<ABallGCActor> BallGCActor;
};

