// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBallModificated.h"
#include "PingPongGoal.h"
#include "PingPongPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/ActorComponents/PlatformModificator.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"


void APingPongBallModificated::SetModification_Implementation(EModificators mod)
{
	Modificator=mod;
	BallColor = PingPongGameState->GetModificatorColor(Modificator);
	SetColor();
}

bool APingPongBallModificated::SetModification_Validate(EModificators mod)
{
	return true;
}

void APingPongBallModificated::BeginPlay()
{
	auto Material = BodyMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
	BodyMesh->SetMaterial(0,DynamicMaterial);
	Super::BeginPlay();
}

void APingPongBallModificated::SetColor()
{
	if(DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("color"),BallColor);	
	}
}

void APingPongBallModificated::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APingPongBallModificated::AddScoreToPlayer(AActor* Player, int Score)
{
	Super::AddScoreToPlayer(Player, PingPongGameState->GetModificatorPoints(Modificator));
}

void APingPongBallModificated::OnPlatformHitModificator_Implementation(FHitResult hitResult)
{
	OnPlatformHitModificator(hitResult);
	if(APingPongPlatform* PingPongPlatform = Cast<APingPongPlatform>(hitResult.GetActor()))
	{
		UActorComponent* ActorComponent = PingPongPlatform->GetComponentByClass(UPlatformModificator::StaticClass());
		if(!ActorComponent) return;
		UPlatformModificator* PlatformModificator = Cast<UPlatformModificator>(ActorComponent);
		if(PlatformModificator)
		{
			if(Modificator==EModificators::Fast)
			{
				PlatformModificator->SetSpeedOfPlatform();
			}
			if(Modificator==EModificators::Slow)
			{
				PlatformModificator->SetSpeedOfPlatform();
			}
			if(Modificator==EModificators::Shrink)
			{
				PlatformModificator->SetPlatformSize();
			}
			
		}
	}
}
