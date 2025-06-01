#pragma once

USTRUCT(BlueprintType)
struct FSelectionOption
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Option")
	FText Label;
};
