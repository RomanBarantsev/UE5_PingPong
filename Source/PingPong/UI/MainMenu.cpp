// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

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
	JoinGame->OnClicked.AddDynamic(this,&UMainMenu::OnJoinGameBtnClicked);
	CreateGame->OnClicked.AddDynamic(this,&UMainMenu::OnCreateGameBtnClicked);
	DisconnectBtn->OnClicked.AddDynamic(this,&UMainMenu::OnDisconnectBtnClicked);
	ResumeGame->OnClicked.AddDynamic(this,&UMainMenu::OnResumeBtnClicked);
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(),0);	
	if (controller->IsPlayerController())
	{
		Controller = Cast<APingPongPlayerController>(controller);
		HUD  = Cast<ABaseHUD>(controller->GetHUD());		
	}
	Super::NativeConstruct();	
}
