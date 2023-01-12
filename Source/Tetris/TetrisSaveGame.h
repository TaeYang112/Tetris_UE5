// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisSaveGame : public USaveGame
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	TArray<int32> ScoreBoard;

};
