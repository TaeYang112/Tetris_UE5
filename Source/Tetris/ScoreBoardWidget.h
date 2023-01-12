// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()
	
	private:
	UPROPERTY()
	class ATetrisGameState* GameState;

	UFUNCTION(BlueprintCallable)
	void UpdateWidget();

	
	protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* FirstScore;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* SecondScore;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ThirdScore;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* FourthScore;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* FifthScore;

};
