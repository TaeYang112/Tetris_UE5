// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetris.h"
#include "GameFramework/GameMode.h"
#include "Cube.h"
#include "TetrisController.h"
#include "TetrisGameState.h"
#include "TetrisGameMode.generated.h"

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_SIZE 100
#define START_LOC_X -450.0f
#define START_LOC_Y -950.0f
enum {
	X = 0,
    Y = 1
};

enum DIRECTION
{
	LEFT = 0,
	RIGHT,
	DOWN
};

struct FPoint
{
	int32 X;
	int32 Y;
};

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void MkNextBlock();


private:
	UPROPERTY()
	ACube* MovingBlockActor[4];

	UPROPERTY()
	ACube* InvBlockActor[4];

	UPROPERTY()
	ACube* NextBlock[4];

	UPROPERTY()
	ATetrisController* PlayerController;
	
	FPoint MovingBlock[4];
	FPoint CurrentLoc;
	int32 CurrentRotation;
	
	int32 TetrisMap[HEIGHT][WIDTH];
	BlockType CurrentType;
	BlockType NextType;
	bool IsGamePlaying;
	
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle SpawnBlockTimerHandle;
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LastTimeTimerHandle;
	
	UPROPERTY()
	TArray<AActor*> BlockActors;

	float LastTime;

public:
	ATetrisGameMode();
	void SummonBlock();
	bool MoveBlock(DIRECTION, FPoint *movBlock = nullptr);
	void RotateBlock(bool bClockwise);
	void EndMoving();
	const FPoint* GetBlockTransform(BlockType type, int32 Rotation);
	bool IsLocationPossible(FPoint location);
	void ShowMap();
	void BreakingRow(int32 Row);
	void PullDown(int32 StartRow);
	void CheckRows();
	void DropBlock();
	void LoopFalling();
	void SyncInvBlock();
	void GameOver();

	
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
    void ReStart();
};
