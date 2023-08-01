// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongHUD.h"
#include "Blueprint/UserWidget.h"


void APingPongHUD::BeginPlay()
{
	CreateWidgets();
	Super::BeginPlay();
	MainScreenWidget->AddToViewport(0);
}

void APingPongHUD::CreateWidgets()
{
	MainScreenWidget = Cast<UMainScreenWidget>(CreateWidget(GetWorld(),MainScreenWidgetSubClass));
	MainMenuWidget = Cast<UMainMenu>(CreateWidget(GetWorld(),MainMenuWidgetSubClass));
	check(MainScreenWidget);
	check(MainMenuWidget);
}

UMainScreenWidget* APingPongHUD::GetMainScreenWidget()
{
	return MainScreenWidget;
}

UMainMenu* APingPongHUD::GetMainMenuWidget()
{
	return MainMenuWidget;
}
