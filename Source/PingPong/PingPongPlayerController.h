// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MainMenu.h"
#include "UI/PingPongHUD.h"
#include "PingPongPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FTransform StartTransform;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class APingPongPlatform> PlatformClass;

	UPROPERTY()
	class APingPongPlatform* Platform;

public:
	APingPongPlayerController();

	UFUNCTION()
	void SetStartTransform(FTransform NewStartTransform);

	UFUNCTION(Server,Reliable,WithValidation)
	void Initialize();
	virtual void PreInitializeComponents() override;
	
	UFUNCTION(Server,Reliable,WithValidation)
	void SpawnPlatform();

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlatformMoveRight(float AxisValue);

	UPROPERTY()
	APingPongHUD* PingPongHUD;

protected:
	UFUNCTION()
	void OpenMenu();
	UPROPERTY()
	UMainMenu* MainMenu;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UMainMenu> MainMenuClass;
};
