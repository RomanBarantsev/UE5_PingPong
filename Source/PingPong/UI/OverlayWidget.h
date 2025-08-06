// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "OverlayWidget.generated.h"

class UPlayerScore;
enum class EPlayersStatus;
class APongPlayerController;
/**
 * 
 */
UCLASS()
class PINGPONG_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* PlayersScoreTable;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WaitingPlayersText;
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ShotButton;
	UPROPERTY(meta=(BindWidget))
	UButton* MenuBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* ExitGame;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BallCost;	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GameOverText;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerScoreWidgetSub;
	UFUNCTION()
	void OnExitGamePushed();
	UFUNCTION()
	void OnShotButtonPressed();
	virtual void NativeConstruct() override;
protected:
	UFUNCTION()
	void OnReadyButtonPushed();	
	UFUNCTION()
	void OnMenuButtonPushed();	
	UPROPERTY()
	APongPlayerController* PingPongPlayerController;
	UPROPERTY()
	TMap<int32,UPlayerScore*> PlayersScoreMap;
public:	
	UFUNCTION()
	void ShowWaitingForPlayers();
	UFUNCTION()
	void SetBallShotCostTextColor(FLinearColor TextColor);
	int32 CountdownTime;
	UFUNCTION()
	void SetCountDownTime(int32 Time);
	FTimerHandle CountDownTimerHandle;
	UFUNCTION()
	void UpdateCountdown();
	UFUNCTION()
	virtual void UpdateScore(int32 playerId, float Score);
	UFUNCTION()
	void UpdatePlayerList();
	UFUNCTION()
	void SetBallColor(FLinearColor Color);
	UFUNCTION()
	void SetBallShotCostText(int32 score);
	UFUNCTION()
	void ShowGameOverText();
	UFUNCTION()
	void AllPlayersConnected();
	UFUNCTION()
	void AllPlayersReady();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundWave* CountdownSound;
public:
	UFUNCTION()
	void PlayCountDownTimer();
};
