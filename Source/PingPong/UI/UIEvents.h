#pragma once
#include "CoreMinimal.h"

UENUM()
enum class EUIStatus : uint8
{
	NONE,
	UILoaded,
	ReadyButtonPressed,
	UIPaused,
	Started
};

DECLARE_MULTICAST_DELEGATE_OneParam(FUIStatusChangedDelegate, EUIStatus);

class FUIEventSystem
{
public:
	static FUIStatusChangedDelegate& OnUIStatusChanged()
	{
		static FUIStatusChangedDelegate DelegateInstance;
		return DelegateInstance;
	}	
};
