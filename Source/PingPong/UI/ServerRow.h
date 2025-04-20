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
	
protected:	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTxt;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayersTxt;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PingTxt;

	virtual void NativeConstruct() override;
};
