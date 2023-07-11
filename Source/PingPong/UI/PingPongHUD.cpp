// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongHUD.h"
#include "Blueprint/UserWidget.h"


void APingPongHUD::BeginPlay()
{
	HUDMainWidowDraw();
	Super::BeginPlay();
}

void APingPongHUD::HUDMainWidowDraw()
{
	auto HUDWidget = CreateWidget(GetWorld(),MainScreenWidget);
	if(HUDWidget) HUDWidget->AddToViewport(0);
}
