// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PongBall.h"
#include "ClassinPongBall.generated.h"

UCLASS()
class PINGPONG_API AClassinPongBall : public APongBall
{
	GENERATED_BODY()

private:
	virtual void OnBallHitAnything(FHitResult hitResult) override;
	virtual void AddScoreToPlayer(AActor* Player) override;
	void ClampReflectionAngel();
public:
	// Sets default values for this actor's properties
	AClassinPongBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
