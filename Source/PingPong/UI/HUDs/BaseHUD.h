// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/Interface.h"
#include "BaseHUD.generated.h"

class APingPongGameState;
class UMainMenu;
class UOverlayWidget;
/**
 * 
 */
UENUM()
enum Widgets
{
	MainMenu,
	Settings,
	ServerList,
	Overlay
};

UCLASS()
class PINGPONG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void SwitchUI(Widgets UIName,int32 zOrder);
	TMap<Widgets,UUserWidget*> UIWidgetsMap;
protected:
	virtual void BeginPlay() override;	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> ServerBrowserWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> MainMenuWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> SettingsWidgetSubClass;

};
