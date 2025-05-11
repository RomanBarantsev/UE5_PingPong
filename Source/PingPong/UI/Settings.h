// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Settings.generated.h"

class UWidgetSwitcher;
class UMenuAudio;
class UGridPanel;
class UButton;
class UComboBoxString;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */

UCLASS()
class PINGPONG_API USettings : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UGridPanel* GridPanel;
	UPROPERTY(meta = (BindWidget))
	UButton* Sound;	
	UPROPERTY(meta = (BindWidget))
	UButton* Video;
public:
	UFUNCTION()
	void OnVideoBtnPushed();
	UFUNCTION()
	void OnSoundBtnPushed();
protected:	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AudioSettingsSubClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> VideoSettingsSubClass;
	UPROPERTY()
	UUserWidget* AudioSettingsWidget;
	UPROPERTY()
	UUserWidget* VideoSettingsWidget;
};
