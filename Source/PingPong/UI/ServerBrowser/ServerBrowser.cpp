// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowser.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "PingPong/GameInstance/Pong_GameInstance.h"

void UServerBrowser::OnConnectPressed()
{
	
}

void UServerBrowser::OnRefreshPressed()
{
	PongGameInstance->GetServersList();
}

void UServerBrowser::OnBackPressed()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UServerBrowser::OnServerListReady(const TArray<FServerInfo>& Servers)
{
	ServerList->ClearChildren();
	for (const auto& Server : Servers)
	{
		UServerRow* RowWidger = CreateWidget<UServerRow>(this,ServerRowSubClass);
		RowWidger->IP = Server.IP;
		RowWidger->Port = Server.Port;
		RowWidger->SetServerName(Server.Name);
		RowWidger->SetCurrentPlayers(Server.CurrentPlayers,Server.MaxPlayers);
		ServerList->AddChild(RowWidger);
	}
}

void UServerBrowser::VisibilityChanged(ESlateVisibility InVisibility)
{
	if (ESlateVisibility::Visible == InVisibility)
	{
		PongGameInstance->GetServersList();
	}
}

void UServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	auto GI = GetGameInstance();
	check(GI);
	PongGameInstance = Cast<UPong_GameInstance>(GI);
	check(PongGameInstance)
	PongGameInstance->OnServerListReady.BindDynamic(this, &UServerBrowser::OnServerListReady);
	RefreshBtn->OnClicked.AddDynamic(this,&UServerBrowser::OnRefreshPressed);
	BackButton->OnClicked.AddDynamic(this,&UServerBrowser::OnBackPressed);
	OnNativeVisibilityChanged.Add(FNativeOnVisibilityChangedEvent::FDelegate::CreateUObject(this, &UServerBrowser::VisibilityChanged));
}
