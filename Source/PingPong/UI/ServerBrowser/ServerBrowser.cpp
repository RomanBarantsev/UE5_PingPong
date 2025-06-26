// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowser.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/GameInstance/Pong_GameInstance.h"

void UServerBrowser::OnConnectPressed()
{
	
}

void UServerBrowser::OnRefreshPressed()
{
	GameInstance->GetServersList();
}

void UServerBrowser::OnBackPressed()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UServerBrowser::ServerListUpdated(const TArray<FServerInfo>& ServerInfos)
{
	ServerList->ClearChildren();
	for (const auto& Server : ServerInfos)
	{
		auto Widget = CreateWidget<UServerRow>(this, ServerRowClass);
		Widget->SetServerName(Server.Name);
		Widget->SetCurrentPlayers(Server.CurrentPlayers,Server.MaxPlayers);
		Widget->port=Server.Port;
		Widget->IP=Server.IP;
		ServerList->AddChild(Widget);
	}
}

void UServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	auto GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		GameInstance = Cast<UPong_GameInstance>(GI);
		if (GameInstance)
		{
			GameInstance->OnServerListReady.BindDynamic(this,&ThisClass::ServerListUpdated);
		}
	}
	RefreshBtn->OnClicked.AddDynamic(this,&UServerBrowser::OnRefreshPressed);
	BackButton->OnClicked.AddDynamic(this,&UServerBrowser::OnBackPressed);
}
