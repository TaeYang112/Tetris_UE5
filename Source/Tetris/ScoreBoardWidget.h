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

	// 위젯 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateWidget();

	
protected:
	
	virtual void NativeConstruct() override;

	// 스코어 보드에서 점수를 표시하는 텍스트 블록
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
