// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/MessageDialog.h"

void UMainMenu::JoinGame_Clicked()
{
	HUD->SwitchUI(Widgets::ServerList,1);
	this->RemoveFromParent();
}

void UMainMenu::NativeConstruct()
{
	JoinGame->OnClicked.AddDynamic(this,&UMainMenu::JoinGame_Clicked);
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (controller->IsPlayerController())
	{
		HUD  = Cast<ABaseHUD>(controller->GetHUD());		
	}
	Super::NativeConstruct();	
}
