// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionBase.h"

#include "SelectionOption.h"
#include "Components/TextBlock.h"

USelectionBase::USelectionBase()
{
	CurrentSelection=0;
	SetIsFocusable(true);
	SetVisibilityInternal(ESlateVisibility::Visible);
}

void USelectionBase::NativeConstruct()
{
	if (Options.Num()==0)
	{
		UE_LOG(LogTemp,Error,TEXT("USelectionBase::No option provided."));
		return;
	}
	UpdateCurrentSelection();
	FCustomWidgetNavigationDelegate NavigationDelegate;
	NavigationDelegate.BindDynamic(this,&USelectionBase::OnNavigation);
	SetNavigationRuleCustom(EUINavigation::Left,NavigationDelegate);
	SetNavigationRuleCustom(EUINavigation::Right,NavigationDelegate);
	Super::NativeConstruct();
}

void USelectionBase::Clear()
{
	Options.Reset();
}

void USelectionBase::AddOption(const FSelectionOption& InOption)
{
	Options.Add(InOption);
	UpdateCurrentSelection();
}

void USelectionBase::SetCurrentSelection(int32 Index)
{
	check(Index>=0 && Index<Options.Num());
	CurrentSelection=Index;
	UpdateCurrentSelection();
}

void USelectionBase::SelectPrevious()
{
	OnNavigation(EUINavigation::Left);
}

void USelectionBase::SelectNext()
{
	OnNavigation(EUINavigation::Right);
}

UWidget* USelectionBase::OnNavigation(EUINavigation InNavigation)
{
	check(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right);
	const auto Direction = InNavigation == EUINavigation::Left ? -1 : 1;
	CurrentSelection+=Direction;
	if (CurrentSelection<0)
	{
		CurrentSelection=Options.Num()-1;
	}
	else if (CurrentSelection>=Options.Num())
	{
		CurrentSelection=0;
	}
	UpdateCurrentSelection();
	OnSelectionChange.ExecuteIfBound(CurrentSelection);
	return this;
}

void USelectionBase::UpdateCurrentSelection()
{
	Label->SetText(Options[CurrentSelection].Label);
}
