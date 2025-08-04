// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/Interface.h"
#include "BaseHUD.generated.h"

class APongGameState;
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
	Overlay,
	Hosting,
	Loading
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetChanged,Widgets,widget);

UCLASS()
class PINGPONG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION()
	bool IsWidgetVisible(Widgets UIName);
	virtual void SwitchUI(Widgets UIName);
	TMap<Widgets,UUserWidget*> UIWidgetsMap;
protected:
	virtual void BeginPlay() override;	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> ServerBrowserWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> LoadingScreenSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> MainMenuWidgetSubClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> SettingsWidgetSubClass;
UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> HostServerWidgetSubClass;
private:
	FOnWidgetChanged OnWidgetChanged;
	UFUNCTION()
	void WidgetChanged(Widgets Widget);
	void WidgetToTMap(Widgets type,TSubclassOf<UUserWidget> subWidget);
};
