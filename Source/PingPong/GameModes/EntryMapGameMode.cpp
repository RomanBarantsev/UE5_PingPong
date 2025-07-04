// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryMapGameMode.h"

#include "PingPong/GameStates/PongGameState.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"
#include "PingPong/PlayerStates/PongPlayerState.h"
#include "PingPong/UI/HUDs/BaseHUD.h"

AEntryMapGameMode::AEntryMapGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController::StaticClass();
	HUDClass = ABaseHUD::StaticClass();
	GameStateClass = AGameState::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
}
