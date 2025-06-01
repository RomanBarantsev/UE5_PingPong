// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionBase.generated.h"

struct FSelectionOption;
class UTextBlock;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class PINGPONG_API USelectionBase : public UUserWidget
{
	GENERATED_BODY()
	USelectionBase();
	virtual void NativeConstruct() override;
	void Clear();
	void AddOption(const FSelectionOption& InOption);
	void SetCurrentSelection(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SelectPrevious();

	UFUNCTION(BlueprintCallable)
	void SelectNext();

	DECLARE_DELEGATE_OneParam(FOnSelectionChange, int);
	FOnSelectionChange OnSelectionChange;

	protected:
	UFUNCTION()
	UWidget* OnNavigation(EUINavigation InNavigation);

	void UpdateCurrentSelection();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FSelectionOption> Options;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UTextBlock> Label;

	int CurrentSelection;
};
