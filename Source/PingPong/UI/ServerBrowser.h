// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerRow.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/NativeWidgetHost.h"
#include "ServerBrowser.generated.h"


/**
 * 
 */
UCLASS()
class PINGPONG_API UServerBrowser : public UUserWidget
{	
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* ServerList;

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
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ClearServerList() const;

	UPROPERTY()
	TSoftClassPtr<UServerRow> ServerRow;
};
