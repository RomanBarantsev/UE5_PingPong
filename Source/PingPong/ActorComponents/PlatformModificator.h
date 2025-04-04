// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformModificator.generated.h"


class APingPongPlatform;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PINGPONG_API UPlatformModificator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformModificator();	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY()
	APingPongPlatform* PingPongPlatform;
	void GetOwningPlatform();
public:
	UFUNCTION()
	void SetReverseControl();
	UFUNCTION()
	void SetPlatformSize();
	UFUNCTION()
	void SetSpeedOfPlatform(int32 Multiplier);
};
