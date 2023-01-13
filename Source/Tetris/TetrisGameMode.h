// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetris.h"
#include "GameFramework/GameMode.h"
#include "Cube.h"
#include "TetrisController.h"
#include "TetrisGameState.h"
#include "TetrisGameMode.generated.h"

// ��Ʈ���� ���� ũ�� ( �� �� �ִ� ����� ���� )
#define WIDTH 10
#define HEIGHT 20

// ��� �Ѱ��� ũ��
#define BLOCK_SIZE 100

// ���� ������ ����� �����Ǵ� ��ġ
#define START_LOC_X -450.0f
#define START_LOC_Y -950.0f

enum {
	X = 0,
    Y = 1
};

// ����
enum DIRECTION
{
	LEFT = 0,
	RIGHT,
	DOWN
};

// ��ǥ ����ü
struct Point
{
	int32 X;
	int32 Y;
};

UCLASS()
class TETRIS_API ATetrisGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// ���� ��ȯ�� ����� ������ �̸����� ���·� ������
	void MakeNextBlock();

private:
	// ���� �������� �ִ� �ҷ�
	UPROPERTY()
	ACube* MovingBlockActor[4];

	// ���� �������� �ִ� ����� ��ġ�� ���� �������ϰ� �����ִ� ���
	UPROPERTY()
	ACube* InvBlockActor[4];

	// ������ ��ȯ�� ���
	UPROPERTY()
	ACube* NextBlock[4];

	// �÷��̾� ��Ʈ�ѷ��� ��� ����
	UPROPERTY()
	ATetrisController* PlayerController;

	// 2���� �迭���� �������� �ִ� ��ϵ��� ��ǥ
	Point MovingBlock[4];

	// ��ϵ��� ������ ( ȸ���� �̵����� ���� )
	Point CurrentLoc;

	// ����� �����·� ���� ȸ���� ���� ( 0 ~ 3 )
	int32 CurrentRotation;

	// ��Ʈ���� ���� ����ִ� ������ �迭
	int32 TetrisMap[HEIGHT][WIDTH];

	// ���� �������� �ִ� ����� Ÿ�� ( T, I, L, O, J )
	BlockType CurrentType;

	// ���� ��ȯ�� ����� Ÿ��
	BlockType NextType;

	// ���� ������ ���������� �÷��� ����
	bool IsGamePlaying;

	// ����� ��ȯ ������ �����ϴ� Ÿ�̸� �ڵ� ( ���� ����ð��� ��������� ������ ª���� )
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle SpawnBlockTimerHandle;

	// ���� �ð��� �����ϴ� Ÿ�̸� �ڵ� ( 1�� ���� )
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LastTimeTimerHandle;

	// ���� ���� ��ȯ�� ��� ���͸� �����ϴ� ����Ʈ
	UPROPERTY()
	TArray<AActor*> BlockActors;

	// ������ �귯�� �ð��� �����ϴ� ����
	float LastTime;

public:
	ATetrisGameMode();

	// ��� ����
	void SummonBlock();

	// ����� DIRECTION �������� ������
	bool MoveBlock(DIRECTION, Point *movBlock = nullptr);

	// ����� �ð� / �ݽð� �������� ȸ��
	void RotateBlock(bool bClockwise);

	// ����� �̵��� ������ ���� ��� ��ȯ
	void EndMoving();

	// type�� �´� ��Ʈ���� ����� ��ǥ�� ��ȯ
	const Point* GetBlockTransform(BlockType type, int32 Rotation);

	// �ش� ��ǥ�� ����� ��ġ�� �� �ִ��� ��ȯ
	bool IsLocationPossible(Point location);

	// Row�� �ش��ϴ� �������� ����
	void BreakingRow(int32 Row);

	// StartRow���� ���� �ִ� ��� ����� ��ĭ�� ������ ����
	void PullDown(int32 StartRow);

	// �ش� ���� �ϼ��Ǿ����� �˻�
	void CheckRows();

	// ���� �����̰� �ִ� ��Ʈ���� ����� ������ �� ������ ����
	void DropBlock();

	// Ÿ�̸ӿ� ���� �ݺ������� ����� ������ ��ĭ�� ����
	void LoopFalling();

	// ���� ����� ���� �����̰� �ִ� ��ϰ� ����
	void SyncInvBlock();

	// ���� ����
	void GameOver();

	// ���� ����
	UFUNCTION(BlueprintCallable)
	void StartGame();

	// �����
	UFUNCTION(BlueprintCallable)
    void ReStart();
};
