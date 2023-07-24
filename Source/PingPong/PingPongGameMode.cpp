// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameMode.h"

#include "GameplayTagContainer.h"
#include "PingPongPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PingPongPlayerPawn.h"

APingPongGameMode::APingPongGameMode()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
}

void APingPongGameMode::BeginPlay()
{
	OnMatchStateChanged.AddUObject(this,&APingPongGameMode::MatchStateChanged);
	Super::BeginPlay();
}

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{
	
	UWorld* world = GetWorld();
	if(world && (!Player1Start || !Player2Start))
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(), foundActors);
		if(foundActors.Num() > 0) Player1Start = (APlayerStart*)foundActors[0];
		if(foundActors.Num() > 1) Player2Start = (APlayerStart*)foundActors[1];
	}
	APingPongPlayerController * currPlayer = NULL;
	APlayerStart* startPos = NULL;
	if(Player1 == NULL)
	{
		Player1 = (APingPongPlayerController*)NewPlayer;
		currPlayer = Player1;
		startPos = Player1Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player1"));
		}
	else if(Player2 == NULL)
	{
		Player2 = (APingPongPlayerController*)NewPlayer;
		currPlayer = Player2;
		startPos = Player2Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player2"));
		}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PingPongGameMode: There are already two players in the game. New connections are not possible"));
		return;
		}
	APingPongPlayerPawn* newPawn = Cast<APingPongPlayerPawn>(NewPlayer->GetPawn());
	if(!newPawn)
	{
		newPawn = world->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
	}
	if(startPos && newPawn)
	{
		newPawn->SetActorLocation(startPos->GetActorLocation());
		newPawn->SetActorRotation(startPos->GetActorRotation());
		NewPlayer->Possess(newPawn);
		currPlayer->SetStartTransform(startPos->GetActorTransform());
		currPlayer->Initialize();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
	}
	APingPongPlayerController* PingPongPlayerController = Cast<APingPongPlayerController>(NewPlayer);
	if(PingPongPlayerController)
	{
		PingPongPlayerController->OnPlayerReady.AddUObject(this,&APingPongGameMode::PlayerReady);
	}
	PlayerControllers.Add(PingPongPlayerController);
	if(PlayerControllers.Num()==PlayersCount)
	{
		if(OnMatchStateChanged.IsBound())
			OnMatchStateChanged.Broadcast(MatchState::EnteringMap);
	}
	Super::PostLogin(NewPlayer);    	
}

APlayerController* APingPongGameMode::GetPlayerController(int index)
{
	return PlayerControllers[index];
}

void APingPongGameMode::MatchStateChanged(FName mState)
{
	if(mState==MatchState)
	{
		
	}
}

void APingPongGameMode::PlayerReady()
{
	PlayersReady++;
	if(PlayersReady==PlayersCount)
	{
		OnMatchStateChanged.Broadcast(MatchState::WaitingToStart);
	}
}

