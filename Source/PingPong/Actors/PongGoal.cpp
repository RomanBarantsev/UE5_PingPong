// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGoal.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "PingPong/Pawns/PongPlayerPawn.h"


// Sets default values
APongGoal::APongGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	BoxCollision = CreateDefaultSubobject<UStaticMeshComponent>("BoxCollision");
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/PingPong/Materials/M_Goal2.M_Goal2"));
	if (Material.Succeeded())
	{
		BoxCollision->SetMaterial(0,Material.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxCollisionMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if(BoxCollisionMeshAsset.Succeeded())
	{
		BoxCollision->SetStaticMesh(BoxCollisionMeshAsset.Object);
	}
	SetRootComponent(BoxCollision);
	LightBox = CreateDefaultSubobject<UStaticMeshComponent>("LightBox");
	LightBox->SetupAttachment(BoxCollision);
	LightBox->SetRelativeScale3D(FVector{0.63,0.63,0.63});
	LightBox->SetRelativeLocation(FVector{0,0,-30});
	LightTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("LightTimelineComp"));
	
	//Initialize Brightness Multiplier
	BrightnessMultiplier = 20.0f;	
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if(MeshAsset.Succeeded())
	{
		LightBox->SetStaticMesh(MeshAsset.Object);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,FString{"No Static Mesh for Goal is found"});
	}
	bReplicates=true;
	//TODO make goal blink on touch float blend = 0.5f + FMath::Cos(GetWorld()->TimeSeconds)/2;
}

// Called when the game starts or when spawned
void APongGoal::BeginPlay()
{
	Super::BeginPlay();
	if(!DynamicMaterial)
	{
		auto Material = LightBox->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
		if(!DynamicMaterial)
			return;
		LightBox->SetMaterial(0,DynamicMaterial);		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,FString{"No Dynamic Material for Goal is found"});
	}
	UpdateBrightnessTrack.BindDynamic(this, &APongGoal::UpdateLightBrightness);
	UpdateColorTrack.BindDynamic(this, &APongGoal::UpdateLightColor);
	//If we have a float curve, bind it's graph to our update function
	if (LightBoxFloatCurve)
	{
		LightTimelineComp->AddInterpFloat(LightBoxFloatCurve, UpdateBrightnessTrack);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,FString(TEXT("No FloatCurve for Light")));
	}
	if (LightBoxColorCurve)
	{
		LightTimelineComp->AddInterpLinearColor(LightBoxColorCurve, UpdateColorTrack);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,FString(TEXT("No LightColorCurve for Light")));
	}
}

// Called every frame
void APongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APongGoal::UpdateLightBrightness_Implementation(float BrightnessOutput)
{
	DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"),BrightnessOutput);
}

void APongGoal::UpdateLightColor(FLinearColor ColorOutput)
{
	
}

void APongGoal::LightUpLightBox_Implementation()
{
	LightTimelineComp->PlayFromStart();
}