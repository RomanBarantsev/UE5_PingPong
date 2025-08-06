// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PongPlatform.h"
#include "ClassicPongPlatform.generated.h"

UCLASS()
class PINGPONG_API AClassicPongPlatform : public APongPlatform
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClassicPongPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
