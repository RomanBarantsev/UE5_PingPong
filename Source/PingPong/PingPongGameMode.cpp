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
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(), foundActors);		
		APlayerStart* startPos=Cast<APlayerStart>(foundActors[PlayerControllers.Num()-1]);
		APingPongPlayerPawn* newPawn = Cast<APingPongPlayerPawn>(PingPongPlayerController->GetPawn());
		if(!newPawn)
		{
			newPawn = world->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
		}	
		if(startPos && newPawn)
		{
			newPawn->SetActorLocation(startPos->GetActorLocation());
			newPawn->SetActorRotation(startPos->GetActorRotation());
			NewPlayer->Possess(newPawn);
			PingPongPlayerController->SetStartTransform(startPos->GetActorTransform());
			PingPongPlayerController->Initialize();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
		}
		if(PlayerControllers.Num()==PlayersCount)
		{
			APingPongGameState* PingPongGameState  = GetGameState<APingPongGameState>();			
			if(PingPongGameState)
			{
				PingPongGameState->UpdateCharacterState(EPlayersStatus::AllPlayersConnected);
			}
		}
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongGoal::StaticClass(), foundActors);
		if(foundActors.Num()!=0)
		{
			float distancePrev = 20000.0f;
			AActor* closestGoal=nullptr;
			for (auto FoundActor : foundActors)
			{
				float distance = FVector::Dist2D(newPawn->GetActorLocation(),FoundActor->GetActorLocation());
				if(distance<distancePrev)
				{
					closestGoal=FoundActor;
				}
			}
			if(closestGoal) closestGoal->SetOwner(newPawn);
		}
		
	}	
	Super::PostLogin(NewPlayer);    	
}
