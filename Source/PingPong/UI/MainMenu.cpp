// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameInstance/NetworkGameInstance.h"

class UNetworkGameInstance;

void UMainMenu::OnJoinGameBtnClicked()
{	
	if (NetworkGI)
	{
		NetworkGI->FindOnlineGames();
	}	
	//HUD->SwitchUI(Widgets::ServerList,1);
}

void UMainMenu::OnCreateGameBtnClicked()
{
	if (NetworkGI)
	{
		NetworkGI->StartOnlineGame();
	}	
}

void UMainMenu::OnDisconnectBtnClicked()
{
	if (NetworkGI)
	{
		NetworkGI->DestroySessionAndLeaveGame();
	}
}

void UMainMenu::NativeConstruct()
{
	auto GameInstance = GetGameInstance();
	NetworkGI = Cast<UNetworkGameInstance>(GameInstance);
	JoinGame->OnClicked.AddDynamic(this,&UMainMenu::OnJoinGameBtnClicked);
	CreateGame->OnClicked.AddDynamic(this,&UMainMenu::OnCreateGameBtnClicked);
	DisconnectBtn->OnClicked.AddDynamic(this,&UMainMenu::OnDisconnectBtnClicked);
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (controller->IsPlayerController())
	{
		HUD  = Cast<ABaseHUD>(controller->GetHUD());		
	}
	Super::NativeConstruct();	
}
