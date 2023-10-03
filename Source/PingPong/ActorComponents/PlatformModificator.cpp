// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformModificator.h"

#include "PingPong/Actors/PingPongPlatform.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"

// Sets default values for this component's properties
UPlatformModificator::UPlatformModificator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlatformModificator::BeginPlay()
{
	Super::BeginPlay();
	GetOwningPlatform();
	// ...
	
}


// Called every frame
void UPlatformModificator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlatformModificator::GetOwningPlatform()
{
	AActor* OwningActor = GetOwner();
	PingPongPlatform = Cast<APingPongPlatform>(OwningActor);
	check(PingPongPlatform);
}

void UPlatformModificator::SetReverseControl()
{	
		PingPongPlatform->bInvertedControl=!PingPongPlatform->bInvertedControl;
}

void UPlatformModificator::SetPlatformSize()
{	
		PingPongPlatform->SetActorScale3D(FVector(1,0.5,1));
}

void UPlatformModificator::SetSpeedOfPlatform(int32 Multiplier)
{
	PingPongPlatform->SetSpeedMultiplier(Multiplier);
}
