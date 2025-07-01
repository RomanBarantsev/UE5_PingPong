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
	UTextBlock* TimerText;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* PlayersScoreTable;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WaitingPlayersText;
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta=(BindWidget))
	UButton* MenuBtn;
	UPROPERTY(meta=(BindWidget))
	UBorder* BallColor;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BallCost;	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GameOverText;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerScoreWidget;
	virtual void NativeConstruct() override;
protected:
	UFUNCTION()
	void OnReadyButtonPushed();	
	UFUNCTION()
	void OnMenuButtonPushed();	
	UPROPERTY()
	APingPongPlayerController* PingPongPlayerController;
	UPROPERTY()
	TMap<int32,UPlayerScore*> PlayersScoreMap;
public:	
	UFUNCTION()
	void ShowWaitingForPlayers();
	int32 CountdownTime;
	UFUNCTION()
	void SetCountDownTime(int32 Time);
	FTimerHandle CountDownTimerHandle;
	UFUNCTION()
	void UpdateCountdown();
	UFUNCTION()
	void UpdateScore(int32 playerId, float Score);
	UFUNCTION()
	void UpdatePlayerList();
	UFUNCTION()
	void SetBallSquareColor(FLinearColor Color);
	UFUNCTION()
	void SetBallShotCostText(int32 score);
	UFUNCTION()
	void ShowGameOverText();
	UFUNCTION()
	void AllPlayersConnected();
	UFUNCTION()
	void AllPlayersReady();
};
