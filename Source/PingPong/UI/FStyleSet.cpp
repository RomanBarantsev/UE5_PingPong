#include "FStyleSet.h"

#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
	TSharedPtr<ISlateStyle> Style = FSlateGameResources::New(FName("StyleSet"),
	ScopeToDirectory, ScopeToDirectory);
	return Style;
}

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = nullptr;

void FStyleSet::Initialize()
{
	Shutdown();
	StylePtr = FindStyle("/Game/PingPong/UI/"	);
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void FStyleSet::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}
}

const ISlateStyle& FStyleSet::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}
	return *StylePtr;
}
