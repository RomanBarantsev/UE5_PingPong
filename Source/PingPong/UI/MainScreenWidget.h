// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PingPong/PingPongGameState.h"
#include "PingPong/PingPongPlayerState.h"
#include "MainScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UMainScreenWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextScorePlayer1;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextScorePlayer2;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WaitingPlayersText;
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyButton;
	virtual void NativeConstruct() override;
private:
	int count = 3;
	FTimerHandle CountTimerHandle;
	
private:
	UFUNCTION()
	void SetScoreText(int Player1,int Player2);

protected:
	UPROPERTY()
	APingPongGameState* PingPongGameState;
	UFUNCTION()
	void OnReadyButtonPushed();
	UFUNCTION()
	void OnPlayersStateChanged(EPlayersStatus PlayersStatus);
};
