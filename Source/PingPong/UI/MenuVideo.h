// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInstance.h"
#include "MenuVideo.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class PINGPONG_API UMenuVideo : public UMenuInstance
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
protected:
	
	UPROPERTY()
	UComboBoxString* ResolutionComboBox;
	
};
