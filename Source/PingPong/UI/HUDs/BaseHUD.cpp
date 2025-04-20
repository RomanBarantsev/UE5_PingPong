// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"


void ABaseHUD::SwitchUI(Widgets UIName,int32 zOrder)
{
	if (UIWidgetsMap.Contains(UIName))
	{
		UUserWidget* widget = UIWidgetsMap[UIName];
		if (widget)
		{
			if (widget->IsInViewport())
			{
				widget->RemoveFromParent();
			}
			else
			{
				widget->AddToViewport(zOrder);
			}
		}
	}
}

void ABaseHUD::BeginPlay()
{
	
	Super::BeginPlay();	
	if (MainMenuWidgetSubClass)
	{
		UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(),MainMenuWidgetSubClass);
		UIWidgetsMap.Add(Widgets::MainMenu,MainMenu);
	}	
	if (ServerBrowserWidgetSubClass)
	{
		UUserWidget* ServerBrowser = CreateWidget<UUserWidget>(GetWorld(),ServerBrowserWidgetSubClass);
		UIWidgetsMap.Add(Widgets::ServerList,ServerBrowser);
	}
	if (SettingsWidgetSubClass)
	{
		UUserWidget* Settings = CreateWidget<UUserWidget>(GetWorld(),SettingsWidgetSubClass);	
		UIWidgetsMap.Add(Widgets::Settings,Settings);
	}
}




