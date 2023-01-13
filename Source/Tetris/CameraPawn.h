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
	// ���� ���۽� ȣ��
	virtual void BeginPlay() override;

public:	
	// Ű ���ε�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ����� Ű �Է½� ȣ��
	void LeftRight(float axis);
	void Down(float axis);
	void Drop();
	void RotateRight();
	void RotateLeft();

	// ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	// ��,�� �����ӿ� �ð� ������ �ִ� Ÿ�̸�
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LeftRightTimerHandle;

	// �Ʒ��� ���� �����ӿ� �ð� ������ �ִ� Ÿ�̸�
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle DownTimerHandle;

private:
	// ���Ӹ�� ��ü�� �����ϴ� ����
	UPROPERTY()
	class ATetrisGameMode* TGameMode;

	// �¿�� ������ �� �ִ��� Ȯ���ϴ� ����
	bool bMoveAble;

	// �Ʒ��� ������ �� �ִ��� Ȯ���ϴ� ����
	bool bDownAble;
};
