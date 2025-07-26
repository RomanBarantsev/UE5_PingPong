// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformModificator.h"

#include "PingPong/Actors/PingPongPlatform.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"

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
	if (PingPongPlatform->bInvertedControl)
	{
		PingPongPlatform->bInvertedControl=false;
		return;
	}
	PingPongPlatform->bInvertedControl=!PingPongPlatform->bInvertedControl;
	GetWorld()->GetTimerManager().SetTimer(
			ReverseTimer,
			this,
			&UPlatformModificator::SetReverseControl,
			ReverseDelay,
			false
		);
}

void UPlatformModificator::SetPlatformSize(float Modificator)
{
		auto Scale = PingPongPlatform->GetActorScale3D();
		Scale.Y += Modificator;			
		PingPongPlatform->SetActorScale3D(Scale);
}

void UPlatformModificator::SetSpeedOfPlatform(float Multiplier)
{
	PingPongPlatform->SetSpeedMultiplier(Multiplier);
}
