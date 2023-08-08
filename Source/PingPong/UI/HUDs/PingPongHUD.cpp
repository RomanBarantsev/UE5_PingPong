// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongHUD.h"
#include "Blueprint/UserWidget.h"
#include "PingPong/UI/MainMenu.h"
#include "PingPong/UI/OverlayWidget.h"


void APingPongHUD::BeginPlay()
{
	CreateWidgets();
	Super::BeginPlay();
	OverlayWidget->AddToViewport(0);
}

void APingPongHUD::CreateWidgets()
{
	OverlayWidget = Cast<UOverlayWidget>(CreateWidget(GetWorld(),OverlayWidgetSubClass));
	MainMenuWidget = Cast<UMainMenu>(CreateWidget(GetWorld(),MainMenuWidgetSubClass));
	check(OverlayWidget);
	check(MainMenuWidget);
}

UOverlayWidget* APingPongHUD::GetOverlayWidget()
{
	return OverlayWidget;
}

UMainMenu* APingPongHUD::GetMainMenuWidget()
{
	return MainMenuWidget;
}
