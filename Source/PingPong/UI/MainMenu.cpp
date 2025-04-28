// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameInstance/NetworkGameInstance.h"

class UNetworkGameInstance;

void UMainMenu::JoinGame_Clicked()
{	
	if (NetworkGI)
	{
		NetworkGI->FindOnlineGames();
	}	
	//HUD->SwitchUI(Widgets::ServerList,1);
}

void UMainMenu::CreateGame_Clicked()
{
	if (NetworkGI)
	{
		NetworkGI->StartOnlineGame();
	}	
}

void UMainMenu::NativeConstruct()
{
	auto GameInstance = GetGameInstance();
	NetworkGI = Cast<UNetworkGameInstance>(GameInstance);
	JoinGame->OnClicked.AddDynamic(this,&UMainMenu::JoinGame_Clicked);
	CreateGame->OnClicked.AddDynamic(this,&UMainMenu::CreateGame_Clicked);
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (controller->IsPlayerController())
	{
		HUD  = Cast<ABaseHUD>(controller->GetHUD());		
	}
	Super::NativeConstruct();	
}
