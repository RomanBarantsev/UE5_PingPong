// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

class UTextBlock;
class UButton;
class USizeBox;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

UCLASS()
class PINGPONG_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;
	UPROPERTY(meta = (BindWidget))
	UButton* Button;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void HandleButtonClicked();
public:
	UPROPERTY(EditAnywhere, BlueprintCallable)
	FOnButtonClicked OnButtonClicked;
	UFUNCTION()
    void SetButtonImage(UTexture2D* Texture);
	UFUNCTION()
	void SetButtonText(FString txt);
};
