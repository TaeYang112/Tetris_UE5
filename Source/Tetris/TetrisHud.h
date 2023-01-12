// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TetrisGameState.h"
#include "Blueprint/UserWidget.h"
#include "TetrisHud.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisHud : public UUserWidget
{
	GENERATED_BODY()

public:


private:
	UPROPERTY()
	ATetrisGameState* GameState;
	
protected:
	void UpdateWidget();
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CurrentScore;
};
