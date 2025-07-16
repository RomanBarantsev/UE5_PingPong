// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostGame.generated.h"

class UUniformGridPanel;
class UButton;
class UCheckBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PINGPONG_API UHostGame : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UCheckBox* bIsLocalServer;
	UPROPERTY(meta=(BindWidget))
	UButton* HostServerBtn;
	UFUNCTION()
	void MapChoosed(FString Name);
	FString MapChoosedName="GameMap";
	virtual void NativeConstruct() override;
	UFUNCTION()
	void HostServerBtnClicked();
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* MapsGridPanel;
};
