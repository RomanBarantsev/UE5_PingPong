// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryMapGameMode.h"

#include "PingPong/GameStates/PingPongGameState.h"
#include "PingPong/PlayerControllers/PingPongPlayerController.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "PingPong/UI/HUDs/BaseHUD.h"

AEntryMapGameMode::AEntryMapGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController::StaticClass();
	HUDClass = ABaseHUD::StaticClass();
	GameStateClass = AGameState::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
}
