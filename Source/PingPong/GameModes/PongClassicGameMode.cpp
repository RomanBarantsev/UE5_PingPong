// Fill out your copyright notice in the Description page of Project Settings.


#include "PongClassicGameMode.h"

#include "PingPong/Pawns/ClassicPongPawn.h"

APongClassicGameMode::APongClassicGameMode()
{
	DefaultPawnClass = AClassicPongPawn::StaticClass();
}
