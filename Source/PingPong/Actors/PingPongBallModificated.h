// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingPongBall.h"
#include "PingPongBallModificated.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongBallModificated : public APingPongBall
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	EModificators Modificator = EModificators::None;
	
private:	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY(ReplicatedUsing=SetColor)
	FLinearColor BallColor;
	
public:
	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void SetModification(EModificators mod);
	UFUNCTION()
	void SetColor();

private:	
	UFUNCTION(Server,Reliable)
	void OnPlatformHitModificator(FHitResult hitResult);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	virtual void AddScoreToPlayer(AActor* Player, int Score) override;
};
