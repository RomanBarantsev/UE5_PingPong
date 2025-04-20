// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Actors/PingPongGoal.h"
#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/Pawns/PingPongPlayerPawn.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/HUDs/BaseHUD.h"

APingPongGameMode::APingPongGameMode()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
	HUDClass = ABaseHUD::StaticClass();
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
	SetClosestGoalOwner(Pawn);	
	Super::PostLogin(NewPlayer);
	
}

void APingPongGameMode::Logout(AController* Exiting)
{
	//TODO client which reconnect after close window is desync, and platform stays in level. Some bug need to fix. 
	APingPongPlayerController* PingPongPlayerController = Cast<APingPongPlayerController>(Exiting);
	PingPongGameState = Cast<APingPongGameState>( GetGameState<APingPongGameState>());
	check(PingPongGameState);
	PingPongGameState->GetPlayersControllers().Remove(PingPongPlayerController);
	Super::Logout(Exiting);
}

APingPongPlayerPawn* APingPongGameMode::CreatePawnForController(APingPongPlayerController* PingPongPlayerController,
                                                                UWorld* World)
{
	APingPongPlayerPawn* newPawn = Cast<APingPongPlayerPawn>(PingPongPlayerController->GetPawn());
	if(!newPawn)
	{
		newPawn = World->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
		return newPawn;
	}
	return nullptr;	
}

void APingPongGameMode::SetClosestGoalOwner(APingPongPlayerPawn* PingPongPlayerPawn)
{
	if(!HasAuthority()) return;
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APingPongGoal::StaticClass(),foundActors);
	float lastDistance=20000.0f;
	APingPongGoal* ClosestGoal=nullptr;
	for (auto FoundActor : foundActors)
	{
		APingPongGoal* PongGoal = Cast<APingPongGoal>(FoundActor);
		if(!PongGoal) continue;
		float distance = FVector::Dist2D(PingPongPlayerPawn->GetActorLocation(),PongGoal->GetActorLocation());
		if(distance<lastDistance)
		{
			ClosestGoal=PongGoal;
			lastDistance=distance;
		}		
	}
	ClosestGoal->SetOwner(PingPongPlayerPawn);
}

int APingPongGameMode::GetPlayersCount() const
{
	return PlayersCount;
}


void APingPongGameMode::SetPawnRotationAndLocation_Implementation(APingPongPlayerPawn* PingPongPlayerPawn,
                                                                  APingPongPlayerController* PingPongPlayerController)
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

