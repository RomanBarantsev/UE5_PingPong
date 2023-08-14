// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OverlayWidget.generated.h"

enum class EPlayersStatus;
class APingPongPlayerController;
/**
 * 
 */
UCLASS()
class PINGPONG_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextScorePlayer1;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextScorePlayer2;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextNamePlayer1;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextNamePlayer2;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WaitingPlayersText;
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyButton;
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void SetScoreTextVisible();
	
protected:
	UFUNCTION()
	void OnReadyButtonPushed();	
	UPROPERTY()
	APingPongPlayerController* PingPongPlayerController;
	UPROPERTY()
	TMap<int32,UTextBlock*> PlayersScoreMap;
public:	
	UFUNCTION()
	void ShowWaitingForPlayers();
	UFUNCTION()
	void OnPlayersStateChanged(EPlayersStatus PlayersStatus);	
	UFUNCTION()
	void UpdateCountdown(int32 value);
	UFUNCTION()
	void UpdateScore(int32 playerId, float Score);
	UFUNCTION()
	void SetPlayerScoreVisible(int32 PlayerId);
};
