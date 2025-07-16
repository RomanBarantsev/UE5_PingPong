// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapButton.generated.h"

class USizeBox;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapButtonClicked, FString, Name);

/**
 * 
 */
UCLASS()
class PINGPONG_API UMapButton : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(Meta=(BindWidget))
	USizeBox* SizeBox;
	UPROPERTY(Meta=(BindWidget))
	UButton* ChooseBtn;
	UPROPERTY(Meta=(BindWidget))
	UImage* MapImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;
	UFUNCTION()
	void ChooseBtnClicked();
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
public:	
	FOnMapButtonClicked OnMapButtonClicked;
};
