// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "MenuButton.h"
#include "Components/VerticalBox.h"
#include "HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameInstance/NetworkGameInstance.h"

class UNetworkGameInstance;

void UMainMenu::OnJoinGameBtnClicked()
{
#ifdef UE_EDITOR
		
#else

	if (NetworkGI)
	{
		NetworkGI->FindOnlineGames();
	}	
	//HUD->SwitchUI(Widgets::ServerList,1);
#endif
}

void UMainMenu::OnCreateGameBtnClicked()
{
#ifdef UE_EDITOR
	
#else	
	if (NetworkGI)
	{
		NetworkGI->StartOnlineGame();
	}	
#endif
}

void UMainMenu::OnDisconnectBtnClicked()
{
#ifdef UE_EDITOR

#else
	if (NetworkGI)
	{
		NetworkGI->DestroySessionAndLeaveGame();
	}
#endif
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
