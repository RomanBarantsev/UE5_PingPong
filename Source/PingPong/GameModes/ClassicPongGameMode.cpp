// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicPongGameMode.h"

#include "PingPong/Pawns/ClassicPongPawn.h"

AClassicPongGameMode::AClassicPongGameMode()
{
	DefaultPawnClass = AClassicPongPawn::StaticClass();
}
