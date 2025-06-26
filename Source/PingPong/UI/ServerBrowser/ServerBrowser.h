// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerRow.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ServerBrowser.generated.h"

class UScrollBox;
struct FServerInfo;
class UPong_GameInstance;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PINGPONG_API UServerBrowser : public UUserWidget
{	
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* RefreshBtn;
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
	void ServerListUpdated(const TArray<FServerInfo>& ServerInfos);
	virtual void NativeConstruct() override;
	UPROPERTY()
	UPong_GameInstance* GameInstance;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* ServerList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerRow> ServerRowClass;
};
