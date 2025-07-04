// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "PingPong/GameInstance/NetworkGameInstance.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"
#include "MainMenu.generated.h"

class UMenuButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PINGPONG_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UNetworkGameInstance* NetworkGI; 
	UFUNCTION()
	void OnJoinGameBtnClicked();
	UFUNCTION()
	void OnCreateGameBtnClicked();
	UFUNCTION()
	void OnDisconnectBtnClicked();
	UFUNCTION()
	void OnResumeBtnClicked();
	UFUNCTION()
	void OnSettingsBtnClicked();
	UFUNCTION()
	void OnQuitButtonPressed();
	
	UPROPERTY(meta=(BindWidget))
	UMenuButton* JoinGame;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* JoinLocalGame;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* DisconnectBtn;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* CreateGame;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* CreateLocalGame;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* Settings;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* Quit;
	UPROPERTY(meta=(BindWidget))
	UMenuButton* ResumeGame;
	UPROPERTY(meta=(BindWidget))
	UBackgroundBlur* Blur;
	UPROPERTY()
	ABaseHUD* HUD;
	UPROPERTY()
	APongPlayerController* Controller;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
public:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool bIsEditor=false;
};
