// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PingPongGameState.h"


void ABaseHUD::SwitchUI(Widgets UIName)
{
	if (UIWidgetsMap.Contains(UIName))
	{
		UUserWidget* widget = UIWidgetsMap[UIName];
		if (widget)
		{
			if (widget->GetVisibility()==ESlateVisibility::Visible)
			{
				widget->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
			else
			{
				widget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void ABaseHUD::BeginPlay()
{
	OnWidgetChanged.AddDynamic(this,&ABaseHUD::WidgetChanged);
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
	for (auto [type,widget] : UIWidgetsMap)
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
		widget->AddToViewport(1);
		if (type==Widgets::MainMenu && GetWorld()->GetMapName()=="EntryMap")
			widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::WidgetChanged(Widgets widget)
{
	//widget==Widgets::MainMenu?  : SwitchUI((Widgets::MainMenu),1);
	
}


