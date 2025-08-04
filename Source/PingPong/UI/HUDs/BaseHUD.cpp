// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameStates/PongGameState.h"


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
	//TODO Make it simple - not repiting everything
	if (MainMenuWidgetSubClass)
	{
		WidgetToTMap(Widgets::MainMenu,MainMenuWidgetSubClass);
	}	
	if (ServerBrowserWidgetSubClass)
	{
		WidgetToTMap(Widgets::ServerList,ServerBrowserWidgetSubClass);
	}
	if (SettingsWidgetSubClass)
	{
		WidgetToTMap(Widgets::Settings,SettingsWidgetSubClass);
	}
	if (HostServerWidgetSubClass)
	{
		WidgetToTMap(Widgets::Hosting,HostServerWidgetSubClass);
	}
	if (LoadingScreenSubClass)
	{
		WidgetToTMap(Widgets::Loading,LoadingScreenSubClass);
	}
	for (auto [type,widget] : UIWidgetsMap)
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
		widget->AddToViewport(1);
		if (type==Widgets::MainMenu && GetWorld()->GetMapName()=="EntryMap")
			widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseHUD::WidgetChanged(Widgets Widget)
{
	//widget==Widgets::MainMenu?  : SwitchUI((Widgets::MainMenu),1);
	
}

void ABaseHUD::WidgetToTMap(Widgets type, TSubclassOf<UUserWidget> subWidget)
{
	if (subWidget)
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(),subWidget);
		UIWidgetsMap.Add(type,widget);
	}
}

bool ABaseHUD::IsWidgetVisible(Widgets UIName)
{
	if (UIWidgetsMap.Contains(UIName))
	{
		UUserWidget* widget = UIWidgetsMap[UIName];
		if (widget)
		{
			if (widget->GetVisibility()==ESlateVisibility::Visible)
				return true;
			else
				return false;
		}
	}
	return false;
}



