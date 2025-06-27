// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
public:	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTxt;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayersTxt;
	UPROPERTY()
	FString IP;
	UPROPERTY()
	int port;
	void SetCurrentPlayers(int Players);
	void SetServerName(FString Name);
	virtual void NativeConstruct() override;
};
