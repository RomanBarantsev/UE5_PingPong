// Fill out your copyright notice in the Description page of Project Settings.


#include "Pong_GameInstance.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "SocketSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PingPong/Settings/Pong_GameUserSettings.h"

void UPong_GameInstance::OnServerListGet(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse,
                                         bool bArg)
{
	if (!bArg) return;
	TSharedPtr<FJsonObject> Root;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) return;

	const TArray<TSharedPtr<FJsonValue>>* ServerList;
	if (!Root->TryGetArrayField("servers", ServerList)) return;
	
	TArray<FServerInfo> Servers;

	for (const TSharedPtr<FJsonValue>& Entry : *ServerList)
	{
		auto Obj = Entry->AsObject();
		if (!Obj.IsValid()) continue;

		FString IPPort = Obj->GetStringField("ip");
		FString Name = Obj->GetStringField("name");
		int32 Current = Obj->GetIntegerField("current");
		int32 Max = Obj->GetIntegerField("max");

		FString IP, PortStr;
		IPPort.Split(":", &IP, &PortStr);

		FServerInfo Server;
		Server.IP = IP;
		Server.Port = FCString::Atoi(*PortStr);
		Server.CurrentPlayers = Current;
		Server.MaxPlayers = Max;
		Server.Name= Name;

		Servers.Add(Server);
	}
	OnServerListReady.ExecuteIfBound(Servers);
}

void UPong_GameInstance::GetServersList()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	FString req =  FString::Printf(TEXT("http://%s:18080/servers"),*PublicServerAddress);
	Request->SetURL(req);
	Request->SetVerb("GET");
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindUObject(this,&ThisClass::OnServerListGet);
}

void UPong_GameInstance::OnCreateHostCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	FString ResponseStr = Response->GetContentAsString();
	FString ServerAddressURL;
	FString QueryParams;

	// Split IP and query
	ResponseStr.Split(TEXT("?"), &ServerAddressURL, &QueryParams);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{		
		PC->ClientTravel(ServerAddressURL, ETravelType::TRAVEL_Absolute);
	}
	// Now split and parse params
	TMap<FString, FString> ParsedParams;
	TArray<FString> Pairs;
	QueryParams.ParseIntoArray(Pairs, TEXT("?"), true);
	
	for (const FString& Pair : Pairs)
	{
		FString Key, Value;
		if (Pair.Split(TEXT("="), &Key, &Value))
		{
			ParsedParams.Add(Key, Value);
		}
	}

	// ✅ Usage
	FString PlayerName = ParsedParams.Contains("PlayerName") ? ParsedParams["PlayerName"] : TEXT("");
	FString IDPlayer   = ParsedParams.Contains("IDPlayer")   ? ParsedParams["IDPlayer"]   : TEXT("");
	FString Color      = ParsedParams.Contains("Color")      ? ParsedParams["Color"]      : TEXT("");
}

void UPong_GameInstance::CreateHost(FString map,FString serverName,uint32 id)
{	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	FString req = FString::Printf(TEXT("http://%s:18080/connect?name=%s&id=%s"),*PublicServerAddress,*FString::FromInt(id),*serverName);
	Request->SetURL(req);
	Request->SetVerb("GET");
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindUObject(this,&ThisClass::OnCreateHostCompleted);	
}

void UPong_GameInstance::PlayersUpdate()
{
	auto GM = UGameplayStatics::GetGameMode(GetWorld());
	if (!GM) return;
	int32 PlayerCount = GM->GetNumPlayers();
	int32 Port = GM->GetWorld()->URL.Port;
	
	FString Url = FString::Printf(TEXT("http://%s:18080/update?port=%d&players=%d"),*CrowServerAddress, Port, PlayerCount);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void UPong_GameInstance::HostShutdown()
{
	auto GM = UGameplayStatics::GetGameMode(GetWorld());
	if (!GM) return;
	int32 Port = GM->GetWorld()->URL.Port;
	FString Url = FString::Printf(TEXT("http://%s:18080/terminated?port=%d"),*CrowServerAddress, Port);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void UPong_GameInstance::Init()
{
	Settings = GEngine->GameUserSettings;
	if (Settings)
	{
		Settings->LoadConfig();	
		Pong_Settings = Cast<UPong_GameUserSettings>(Settings);
		check(Pong_Settings);	
	}
	else
	{
		UKismetSystemLibrary::QuitGame(GetWorld(),GetPrimaryPlayerController(),EQuitPreference::Quit,true);
		UKismetSystemLibrary::LogString(TEXT("No Settings class in GameInstance class"), true);
	}
	Super::Init();
}

void UPong_GameInstance::Shutdown()
{
	Settings->SaveConfig();	
	Super::Shutdown();
}