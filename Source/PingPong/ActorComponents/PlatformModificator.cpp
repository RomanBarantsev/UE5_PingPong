// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformModificator.h"

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

	// ...
	
}


// Called every frame
void UPlatformModificator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlatformModificator::SetReverseControl()
{
	
}

void UPlatformModificator::SetPlatformSize()
{
	
}

void UPlatformModificator::SetSpeedOfPlatform()
{
	
}
