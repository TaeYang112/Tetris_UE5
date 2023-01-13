// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetris.h"
#include "GameFramework/GameMode.h"
#include "Cube.h"
#include "TetrisController.h"
#include "TetrisGameState.h"
#include "TetrisGameMode.generated.h"

// 테트리스 판의 크기 ( 들어갈 수 있는 블록의 개수 )
#define WIDTH 10
#define HEIGHT 20

// 블록 한개의 크기
#define BLOCK_SIZE 100

// 게임 내에서 블록이 생성되는 위치
#define START_LOC_X -450.0f
#define START_LOC_Y -950.0f

enum {
	X = 0,
    Y = 1
};

// 방향
enum DIRECTION
{
	LEFT = 0,
	RIGHT,
	DOWN
};

// 좌표 구조체
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

	// 다음 소환될 블록을 생성후 미리보기 형태로 보여줌
	void MakeNextBlock();

private:
	// 현재 떨어지고 있는 불록
	UPROPERTY()
	ACube* MovingBlockActor[4];

	// 현재 떨어지고 있는 블록이 설치될 곳을 반투명하게 보여주는 블록
	UPROPERTY()
	ACube* InvBlockActor[4];

	// 다음에 소환될 블록
	UPROPERTY()
	ACube* NextBlock[4];

	// 플레이어 컨트롤러를 담는 변수
	UPROPERTY()
	ATetrisController* PlayerController;

	// 2차원 배열에서 떨어지고 있는 블록들의 좌표
	Point MovingBlock[4];

	// 블록들의 기준점 ( 회전과 이동시의 기준 )
	Point CurrentLoc;

	// 블록이 원상태로 부터 회전한 정도 ( 0 ~ 3 )
	int32 CurrentRotation;

	// 테트리스 맵을 담고있는 이차원 배열
	int32 TetrisMap[HEIGHT][WIDTH];

	// 현재 떨어지고 있는 블록의 타입 ( T, I, L, O, J )
	BlockType CurrentType;

	// 다음 소환될 블록의 타입
	BlockType NextType;

	// 현재 게임이 진행중인지 플래그 변수
	bool IsGamePlaying;

	// 블록의 소환 간격을 관리하는 타이머 핸들 ( 게임 진행시간이 길어질수록 간격이 짧아짐 )
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle SpawnBlockTimerHandle;

	// 게임 시간을 측정하는 타이머 핸들 ( 1초 간격 )
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LastTimeTimerHandle;

	// 게임 내에 소환된 블록 액터를 저장하는 리스트
	UPROPERTY()
	TArray<AActor*> BlockActors;

	// 게임이 흘러간 시간을 저장하는 변수
	float LastTime;

public:
	ATetrisGameMode();

	// 블록 생성
	void SummonBlock();

	// 블록을 DIRECTION 방향으로 움직임
	bool MoveBlock(DIRECTION, Point *movBlock = nullptr);

	// 블록을 시계 / 반시계 방향으로 회전
	void RotateBlock(bool bClockwise);

	// 블록의 이동을 끝내고 다음 블록 소환
	void EndMoving();

	// type에 맞는 테트리스 블록의 좌표를 반환
	const Point* GetBlockTransform(BlockType type, int32 Rotation);

	// 해당 좌표에 블록이 배치될 수 있는지 반환
	bool IsLocationPossible(Point location);

	// Row에 해당하는 가로줄을 제거
	void BreakingRow(int32 Row);

	// StartRow부터 위에 있는 모든 블록을 한칸씩 밑으로 내림
	void PullDown(int32 StartRow);

	// 해당 줄이 완성되었는지 검사
	void CheckRows();

	// 현재 움직이고 있는 테트리스 블록을 가능한 맨 밑으로 내림
	void DropBlock();

	// 타이머에 의해 반복적으로 블록을 밑으로 한칸씩 내림
	void LoopFalling();

	// 투명 블록을 현재 움직이고 있는 블록과 맞춤
	void SyncInvBlock();

	// 게임 오버
	void GameOver();

	// 게임 시작
	UFUNCTION(BlueprintCallable)
	void StartGame();

	// 재시작
	UFUNCTION(BlueprintCallable)
    void ReStart();
};
