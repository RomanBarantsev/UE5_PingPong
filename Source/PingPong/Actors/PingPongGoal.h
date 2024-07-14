// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveEvaluation.h"
#include "GameFramework/Actor.h"
#include "PingPongGoal.generated.h"

class APingPongPlayerPawn;
UCLASS()
class PINGPONG_API APingPongGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APingPongGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* LightBox;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	
	//TimelineComponent to animate the point light component.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* LightTimelineComp;
	
	//Track used for Pointlight brightness
	FOnTimelineFloat UpdateBrightnessTrack;

	//Track used for Pointlight color
	FOnTimelineLinearColor UpdateColorTrack;

	//Function to Update Light's brightness relative to UpdateBrightnessTrack
	UFUNCTION(NetMulticast,Reliable)
	void UpdateLightBrightness(float BrightnessOutput);

	//Function to Update Light's color relative to UpdateColorTrack
	UFUNCTION()
	void UpdateLightColor(FLinearColor ColorOutput);

	UPROPERTY(EditDefaultsOnly)
	float BrightnessMultiplier;
public:

	UPROPERTY(EditAnywhere)
	UCurveFloat* LightBoxFloatCurve;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* LightBoxColorCurve;

	UFUNCTION(Server,Reliable)
	void LightUpLightBox();
};
