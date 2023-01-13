// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "TetrisGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;
    
    // ������Ʈ �Ҵ�
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

    RootComponent = Camera;

    // ���� �ʱ�ȭ
    bMoveAble = true;
    bDownAble = true;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
    TGameMode = Cast<ATetrisGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


// �Է� Ű ���ε�
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis(TEXT("Down"), this, &ACameraPawn::Down);
    PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &ACameraPawn::Drop);
    PlayerInputComponent->BindAction(TEXT("RotateLeft"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateLeft);
    PlayerInputComponent->BindAction(TEXT("RotateRight"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateRight);
    PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ACameraPawn::LeftRight);
}


// ����Ű �Է� ( <-, -> )
void ACameraPawn::LeftRight(float axis)
{
    // ������ �� �ִٸ�
    if(bMoveAble)
    {
        // �������� ��ĭ �̵�
        if (axis == 1)
        {
            TGameMode->MoveBlock(DIRECTION::LEFT);
        }
        // ���������� ��ĭ �̵�
        else if (axis == -1)
        {
            TGameMode->MoveBlock(DIRECTION::RIGHT);
        }
        else
            return;

        // 0.15�ʵ��� ���������̰� ��
        bMoveAble = false;
        GetWorld()->GetTimerManager().SetTimer(LeftRightTimerHandle, FTimerDelegate::CreateLambda(
            [this]()
            {
                bMoveAble = true;
            }
        ),0.15f , false);
    }
}

// �Ʒ� ����Ű �Է�
void ACameraPawn::Down(float axis)
{
    // �Ʒ�Ű�� �Է� �����ϴٸ�
    if(bDownAble)
    {
        // �Ʒ��� ��ĭ �̵�
        if (axis == 1)
        {
            TGameMode->MoveBlock(DIRECTION::DOWN);
        }
        else return;

        // 0.15�ʵ��� �Ʒ��� �������̰� ��
        bDownAble = false;
        GetWorld()->GetTimerManager().SetTimer(DownTimerHandle, FTimerDelegate::CreateLambda(
            [this]()
            {
                bDownAble = true;
            }
        ),0.15f , false);
    }
}

// DropŰ �Է�
void ACameraPawn::Drop()
{
    // ��Ʈ���� ����� ������ �� �Ʒ��� ����
    TGameMode->DropBlock();
}

// �ð� ���� ȸ�� Ű �Է�
void ACameraPawn::RotateRight()
{
    // �ð� �������� ����� ȸ��
    TGameMode->RotateBlock(true);
}

// �ݽð� ���� ȸ�� Ű �Է�
void ACameraPawn::RotateLeft()
{
    // �ݽð� �������� ����� ȸ��
    TGameMode->RotateBlock(false);
}