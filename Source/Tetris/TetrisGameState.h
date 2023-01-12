// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TetrisGameState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnScoreChangedDelegate);
UCLASS()
class TETRIS_API ATetrisGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	ATetrisGameState();
	void AddScore(const int32 NewScore);
	void SaveScore();
	int32 GetScore() const;
	void ClearScore();

	
	FOnScoreChangedDelegate FOnScoreChanged;
private:
	UPROPERTY(Transient,VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	int32 Score;

	UPROPERTY()
	class UTetrisSaveGame* SaveGameInstance;
};
