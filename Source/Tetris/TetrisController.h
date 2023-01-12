// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetrisController.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisController : public APlayerController
{
	GENERATED_BODY()


	protected:
	ATetrisController();
	virtual void BeginPlay() override;

	
	private:
	UPROPERTY()
	class UTetrisHud* HUDWidget;
	UPROPERTY()
	class UUserWidget* MainWidget;
	UPROPERTY()
	class UUserWidget* GameOverWidget;

	TSubclassOf<UTetrisHud> HudClass;
	TSubclassOf<UUserWidget> MainWidgetClass;
	TSubclassOf<UUserWidget> OverWidgetClass;

	FInputModeGameOnly InputMode;
	FInputModeUIOnly UIMode;

	bool IsReady;
	public:

	UFUNCTION(BlueprintCallable)
	void GameStart();
	void ShowOverWidget();
	void ReadyToWidget();
	void ShowMainWidget();
};
