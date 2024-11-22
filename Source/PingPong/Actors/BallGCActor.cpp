// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGCActor.h"

#include "GeometryCollection/GeometryCollectionCache.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ABallGCActor::ABallGCActor()
{
	ConstructorHelpers::FObjectFinder<UGeometryCollection> GCAsset(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/PingPong/Blueprints/BP_PingPongBall_GeometryCollection.BP_PingPongBall_GeometryCollection'"));
	if(GCAsset.Succeeded())
	{
		GetGeometryCollectionComponent()->SetRestCollection(GCAsset.Object);			
	}
	SetLifeSpan(10);
}

void ABallGCActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollectionComponent->AddImpulse(FVector(FMath::RandRange(500,1000),FMath::RandRange(500,1000),0),NAME_None,true);
}

void ABallGCActor::SetMaterialColor_Implementation(FLinearColor color)
{	
	if(!DynamicMaterial)
	{
		auto Material = GetGeometryCollectionComponent()->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
		GetGeometryCollectionComponent()->SetMaterial(0,DynamicMaterial);
	}
	DynamicMaterial->SetVectorParameterValue(TEXT("color"),color);
}
