// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "PingPong/PlayerStates/PingPongPlayerState.h"
#include "GameSettings.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UGameSettings : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UEditableText* Nickname;
	UPROPERTY()
	APingPongPlayerState* PlayerState;
protected:
	UFUNCTION()
	void TextCommited(const FText& Text, ETextCommit::Type CommitMethod);
	virtual void NativeConstruct() override;
};
