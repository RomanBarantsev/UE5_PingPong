// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerRow.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ServerBrowser.generated.h"


struct FServerInfo;
class UPong_GameInstance;
/**
 * 
 */
UCLASS()
class PINGPONG_API UServerBrowser : public UUserWidget
{	
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* ServerList;

	UPROPERTY(meta=(BindWidget))
	UButton* RefreshBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* ConnectBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;
	UFUNCTION()
	void OnConnectPressed();
	UFUNCTION()
	void OnRefreshPressed();
	UFUNCTION()
	void OnBackPressed();
private:
	UFUNCTION()
	void OnServerListReady(const TArray<FServerInfo>& Servers);
	virtual void NativeConstruct() override;	

	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerRow> ServerRowSubClass;
	UPROPERTY()
	UPong_GameInstance* PongGameInstance;
};