// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuButton.h"
#include "GameFramework/PlayerState.h"
#include "HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameInstance/NetworkGameInstance.h"
#include "PingPong/GameInstance/Pong_GameInstance.h"

void UMainMenu::OnJoinGameBtnClicked()
{
	if (!GIsEditor && !GIsPlayInEditorWorld && !IsRunningDedicatedServer() && IsRunningGame())
	{
		HUD->SwitchUI(Widgets::ServerList);
	}
}

void UMainMenu::OnCreateGameBtnClicked()
{
	if (!GIsEditor && !GIsPlayInEditorWorld && !IsRunningDedicatedServer() && IsRunningGame())
	{
		auto GI=UGameplayStatics::GetGameInstance(GetWorld());	
		if (GI)
		{
			UPong_GameInstance* Pong_GameInstance = Cast<UPong_GameInstance>(GI);
			if (Pong_GameInstance)
			{
				auto PS = UGameplayStatics::GetPlayerState(GetWorld(),0);
				if (PS)
				{
					Pong_GameInstance->CreateHost("GameMap",PS->GetPlayerName(),GetUniqueID());
				}			
			}
		}
	}
}

void UMainMenu::OnDisconnectBtnClicked()
{
	NetworkGI->DestroySessionAndLeaveGame();
}

void UMainMenu::OnResumeBtnClicked()
{
	Controller->OpenMenu();
}

void UMainMenu::OnSettingsBtnClicked()
{
	HUD->SwitchUI(Widgets::Settings);
}

void UMainMenu::OnQuitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,true);
}

void UMainMenu::NativeConstruct()
{
#ifdef UE_EDITOR
	bIsEditor=true;
#endif	
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (NetMode==NM_ListenServer || NetMode==NM_Client)
	{
		DisconnectBtn->SetVisibility(ESlateVisibility::Visible);
		ResumeGame->SetVisibility(ESlateVisibility::Visible);
		JoinGame->SetVisibility(ESlateVisibility::Collapsed);
		CreateGame->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		DisconnectBtn->SetVisibility(ESlateVisibility::Collapsed);
		ResumeGame->SetVisibility(ESlateVisibility::Collapsed);
		JoinGame->SetVisibility(ESlateVisibility::Visible);
		CreateGame->SetVisibility(ESlateVisibility::Visible);
	}
	auto GameInstance = GetGameInstance();
	NetworkGI = Cast<UNetworkGameInstance>(GameInstance);
	JoinGame->OnButtonClicked.AddDynamic(this,&ThisClass::OnJoinGameBtnClicked);	
	CreateGame->OnButtonClicked.AddDynamic(this,&ThisClass::OnCreateGameBtnClicked);
	DisconnectBtn->OnButtonClicked.AddDynamic(this,&ThisClass::OnDisconnectBtnClicked);
	ResumeGame->OnButtonClicked.AddDynamic(this,&ThisClass::OnResumeBtnClicked);
	Settings->OnButtonClicked.AddDynamic(this,&ThisClass::OnSettingsBtnClicked);
	Quit->OnButtonClicked.AddDynamic(this,&ThisClass::OnQuitButtonPressed);
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(),0);	
	if (controller->IsPlayerController())
	{
		Controller = Cast<APingPongPlayerController>(controller);
		HUD  = Cast<ABaseHUD>(controller->GetHUD());		
	}
	Super::NativeConstruct();	
}

void UMainMenu::NativePreConstruct()
{
	JoinGame->SetButtonText(TEXT("Join Game"));
	CreateGame->SetButtonText(TEXT("Create Game"));
	DisconnectBtn->SetButtonText(TEXT("Disconnect"));
	ResumeGame->SetButtonText(TEXT("Resume Game"));
	Settings->SetButtonText(TEXT("Settings"));
	Quit->SetButtonText(TEXT("Quit"));
	Super::NativePreConstruct();
}
