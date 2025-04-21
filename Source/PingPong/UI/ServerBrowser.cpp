// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowser.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"

void UServerBrowser::OnConnectPressed()
{
	
}

void UServerBrowser::OnRefreshPressed()
{
	ClearServerList();
	if (ServerRow.IsValid())
	{
		UClass* loadedClass = ServerRow.Get();
		UServerRow* newInstance = CreateWidget<UServerRow>(this,loadedClass);
		ServerList->AddChild(newInstance);
	}
	
	
}

void UServerBrowser::OnBackPressed()
{
	this->RemoveFromParent();
}

void UServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshBtn->OnClicked.AddDynamic(this,&UServerBrowser::OnRefreshPressed);
	BackButton->OnClicked.AddDynamic(this,&UServerBrowser::OnBackPressed);
}

void UServerBrowser::ClearServerList() const
{
	ServerList->ClearChildren();
}
