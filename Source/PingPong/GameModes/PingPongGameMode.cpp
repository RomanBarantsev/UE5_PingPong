// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameMode.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Actors/PingPongGoal.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/Pawns/PingPongPlayerPawn.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/HUDs/PingPongHUD.h"

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
	PingPongGameState = Cast<APingPongGameState>( GetGameState<APingPongGameState>());
	check(PingPongGameState);
	PingPongGameState->GetPlayersControllers().Add(PingPongPlayerController);
	if(!world) return;
	APingPongPlayerPawn* Pawn = CreatePawnForController( PingPongPlayerController,world);
	SetPawnRotationAndLocation(Pawn,PingPongPlayerController);
	if(PingPongGameState->GetPlayersControllers().Num()==PlayersCount)
	{					
		if(PingPongGameState)
		{
			PingPongGameState->UpdateCharacterState(EPlayersStatus::AllPlayersConnected);
		}
	}
	SetClosestGoalOwner(Pawn);	
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
	APlayerStart* startPos=Cast<APlayerStart>(foundActors[PingPongGameState->GetPlayersControllers().Num()-1]);	
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
				distancePrev=distance;
			}
		}
		if(closestGoal) closestGoal->SetOwner(PingPongPlayerPawn);
	}
}

