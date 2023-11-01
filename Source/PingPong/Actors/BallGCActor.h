// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BallGCActor.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API ABallGCActor : public AGeometryCollectionActor
{
	GENERATED_BODY()
	ABallGCActor();
	virtual void BeginPlay() override;
public:
	UFUNCTION(NetMulticast,Reliable)
	void SetMaterialColor(FLinearColor color);
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
