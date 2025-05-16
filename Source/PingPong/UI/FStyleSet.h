#pragma once

class FStyleSet
{
public:
	static void Initialize();
	static void Shutdown();
	static const ISlateStyle& Get();
private:
	static TSharedPtr<class ISlateStyle> StylePtr;
	
};
