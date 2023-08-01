// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameMode.h"
#include "GameplayTagContainer.h"
#include "PingPongGameState.h"
#include "PingPongPlayerController.h"
#include "PingPongPlayerState.h"
#include "Actors/PingPongGoal.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PingPongPlayerPawn.h"

APingPongGameMode::APingPongGameMode()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
	HUDClass = APingPongHUD::StaticClass();
	GameStateClass = APingPongGameState::StaticClass();
	PlayerStateClass = APingPongPlayerState::StaticClass();
}

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{	
	UWorld* world = GetWorld();
	APingPongPlayerController* PingPongPlayerController = Cast<APingPongPlayerController>(NewPlayer);
	PlayerControllers.Add(PingPongPlayerController);	
	if(world)
	{		
		APingPongPlayerPawn* Pawn = CreatePawnForController( PingPongPlayerController,world);
		SetPawnRotationAndLocation(Pawn,PingPongPlayerController);
		if(PlayerControllers.Num()==PlayersCount)
		{
			APingPongGameState* PingPongGameState  = GetGameState<APingPongGameState>();			
			if(PingPongGameState)
			{
				PingPongGameState->UpdateCharacterState(EPlayersStatus::AllPlayersConnected);
			}
		}
		SetClosestGoalOwner(Pawn);		
	}	
	Super::PostLogin(NewPlayer);    	
}

int APingPongGameMode::GetPlayersCount() const
{
	return PlayersCount;
}

APingPongPlayerPawn* APingPongGameMode::CreatePawnForController(APingPongPlayerController* PingPongPlayerController,UWorld* World) 
{
	APingPongPlayerPawn* newPawn = Cast<APingPongPlayerPawn>(PingPongPlayerController->GetPawn());
	if(!newPawn)
	{
		newPawn = World->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
		return newPawn;
	}
	return nullptr;	
}

void APingPongGameMode::SetPawnRotationAndLocation(APingPongPlayerPawn* PingPongPlayerPawn,APingPongPlayerController* PingPongPlayerController)
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(), foundActors);
	APlayerStart* startPos=Cast<APlayerStart>(foundActors[PlayerControllers.Num()-1]);
	if(startPos && PingPongPlayerPawn)
	{
		PingPongPlayerPawn->SetActorLocation(startPos->GetActorLocation());
		PingPongPlayerPawn->SetActorRotation(startPos->GetActorRotation());
		PingPongPlayerController->Possess(PingPongPlayerPawn);
		PingPongPlayerController->SetStartTransform(startPos->GetActorTransform());
		PingPongPlayerController->Initialize();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
	}
}

void APingPongGameMode::SetClosestGoalOwner(APingPongPlayerPawn* PingPongPlayerPawn)
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongGoal::StaticClass(), foundActors);
	if(foundActors.Num()!=0)
	{
		float distancePrev = 20000.0f;
		AActor* closestGoal=nullptr;
		for (auto FoundActor : foundActors)
		{
			float distance = FVector::Dist2D(PingPongPlayerPawn->GetActorLocation(),FoundActor->GetActorLocation());
			if(distance<distancePrev)
			{
				closestGoal=FoundActor;
			}
		}
		if(closestGoal) closestGoal->SetOwner(PingPongPlayerPawn);
	}
}

TArray<APingPongPlayerController*>& APingPongGameMode::GetPlayersControllers()
{
	return PlayerControllers;
}

