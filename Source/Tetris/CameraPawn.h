#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class TETRIS_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

protected:
	// 게임 시작시 호출
	virtual void BeginPlay() override;

public:	
	// 키 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 사용자 키 입력시 호출
	void LeftRight(float axis);
	void Down(float axis);
	void Drop();
	void RotateRight();
	void RotateLeft();

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	// 좌,우 움직임에 시간 간격을 주는 타이머
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LeftRightTimerHandle;

	// 아래로 가는 움직임에 시간 간격을 주는 타이머
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle DownTimerHandle;

private:
	// 게임모드 객체를 저장하는 변수
	UPROPERTY()
	class ATetrisGameMode* TGameMode;

	// 좌우로 움직일 수 있는지 확인하는 변수
	bool bMoveAble;

	// 아래로 움직일 수 있는지 확인하는 변수
	bool bDownAble;
};
