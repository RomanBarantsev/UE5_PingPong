// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidget.h"

#include "PlayerScore.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/PlayerControllers/PongPlayerController.h"
#include "PingPong/PlayerStates/PongPlayerState.h"

void UOverlayWidget::OnExitGamePushed()
{
	UGameplayStatics::OpenLevel(GetWorld(),"EntryMap",true);
}

void UOverlayWidget::OnShotButtonPressed()
{
	PingPongPlayerController->Fire();
}

void UOverlayWidget::NativeConstruct()
{
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (!PlatformName.Contains(TEXT("Android")))
	{
		MenuBtn->SetVisibility(ESlateVisibility::Hidden);
	}	
	//TODO Make text blocks for dynamic creating with Slate
	ReadyButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnReadyButtonPushed);
	MenuBtn->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnMenuButtonPushed);
	ExitGame->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnExitGamePushed);
	ShotButton->OnPressed.AddUniqueDynamic(this,&UOverlayWidget::OnShotButtonPressed);
	PingPongPlayerController = Cast<APongPlayerController>(GetOwningPlayer());
	check(PingPongPlayerController);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	PingPongPlayerController->SetUIStatus(EUIStatus::UILoaded);
	GameOverText->SetVisibility(ESlateVisibility::Hidden);
	ExitGame->SetVisibility(ESlateVisibility::Hidden);
	ShowWaitingForPlayers();	
	Super::NativeConstruct();
}

void UOverlayWidget::ShowWaitingForPlayers()
{
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	TimerText->SetVisibility(ESlateVisibility::Hidden);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Visible);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(),0));
}

void UOverlayWidget::SetBallShotCostTextColor(FLinearColor TextColor)
{
	BallCost->SetColorAndOpacity(TextColor);
}

void UOverlayWidget::SetCountDownTime(int32 Time)
{
	CountdownTime = Time;
}

void UOverlayWidget::OnReadyButtonPushed()
{
	PingPongPlayerController->SetUIStatus(EUIStatus::ReadyButtonPressed);
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);	
}

void UOverlayWidget::OnMenuButtonPushed()
{
	PingPongPlayerController->OpenMenu();
}

void UOverlayWidget::UpdateCountdown()
{
	if(CountdownTime==0)
	{
		TimerText->SetText(FText::FromString("GO!"));		
	}
	if(CountdownTime<=-1)
	{
		TimerText->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		PingPongPlayerController->SetUIStatus(EUIStatus::Started);
		TimerText->SetText(FText::FromString(""));	
	}
	if(CountdownTime>0)
	{
		TimerText->SetText(FText::AsNumber(CountdownTime));		
	}
	--CountdownTime;
}

void UOverlayWidget::UpdateScore(int32 playerId, float Score)
{
	UPlayerScore* PlayerScoreWidget = PlayersScoreMap.FindRef(playerId);
	if (PlayerScoreWidget)
		PlayerScoreWidget->SetPlayerScore(Score);
}

void UOverlayWidget::UpdatePlayerList()
{
	PlayersScoreTable->ClearChildren();
	auto GS = UGameplayStatics::GetGameState(GetWorld());
	if (GS)
	{
		APongGameState* PongGS = Cast<APongGameState>(GS);
		if (PongGS)
		{
			auto PSs = PongGS->PlayerStates;
			for (auto PS : PSs)
			{
				if (!PS)
					return;
				UUserWidget* score = CreateWidget<UUserWidget>(this,PlayerScoreWidgetSub);	
				UPlayerScore* scoreWidget = Cast<UPlayerScore>(score);
				if (scoreWidget)
				{
					auto name = PS->GetPlayerName();
					scoreWidget->SetPlayerName(name);
					scoreWidget->SetPlayerScore(0);
					PlayersScoreMap.Add(PS->GetPlayerId(),scoreWidget);
					PlayersScoreTable->AddChild(score);
				}	
			}
		}
	}
}

void UOverlayWidget::SetBallColor(FLinearColor Color)
{
	FLinearColor NormalColor = Color;// or any color
	FButtonStyle ButtonStyle = ShotButton->GetStyle();
	// Set the normal state color
	ButtonStyle.Normal.TintColor = FSlateColor(NormalColor);
	// Optional: style for hovered and pressed states
	ButtonStyle.Hovered.TintColor = FSlateColor(NormalColor * 1.2f);
	ButtonStyle.Pressed.TintColor = FSlateColor(NormalColor * 0.8f);
	// Apply the updated style back to the button
	ShotButton->SetStyle(ButtonStyle);
	ShotButton->SetColorAndOpacity(Color);
}

void UOverlayWidget::SetBallShotCostText(int32 score)
{
	BallCost->SetText(FText::AsNumber(score));
}

void UOverlayWidget::ShowGameOverText()
{
	GameOverText->SetVisibility(ESlateVisibility::Visible);
	ExitGame->SetVisibility(ESlateVisibility::Visible);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Collapsed);
	ReadyButton->SetVisibility(ESlateVisibility::Collapsed);
	TimerText->SetVisibility(ESlateVisibility::Collapsed);
}

void UOverlayWidget::AllPlayersConnected()
{
	ReadyButton->SetVisibility(ESlateVisibility::Visible);
	WaitingPlayersText->SetVisibility(ESlateVisibility::Hidden);	
}

void UOverlayWidget::AllPlayersReady()
{	
	TimerText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UOverlayWidget::UpdateCountdown, 1.0f, true);
	PlayCountDownTimer();
}

void UOverlayWidget::PlayCountDownTimer()
{
	PlaySound(CountdownSound);
}


