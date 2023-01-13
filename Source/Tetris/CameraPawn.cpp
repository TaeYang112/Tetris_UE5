// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "TetrisGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;
    
    // 컴포넌트 할당
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

    RootComponent = Camera;

    // 변수 초기화
    bMoveAble = true;
    bDownAble = true;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
    TGameMode = Cast<ATetrisGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


// 입력 키 바인딩
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis(TEXT("Down"), this, &ACameraPawn::Down);
    PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &ACameraPawn::Drop);
    PlayerInputComponent->BindAction(TEXT("RotateLeft"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateLeft);
    PlayerInputComponent->BindAction(TEXT("RotateRight"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateRight);
    PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ACameraPawn::LeftRight);
}


// 방향키 입력 ( <-, -> )
void ACameraPawn::LeftRight(float axis)
{
    // 움직일 수 있다면
    if(bMoveAble)
    {
        // 왼쪽으로 한칸 이동
        if (axis == 1)
        {
            TGameMode->MoveBlock(DIRECTION::LEFT);
        }
        // 오른쪽으로 한칸 이동
        else if (axis == -1)
        {
            TGameMode->MoveBlock(DIRECTION::RIGHT);
        }
        else
            return;

        // 0.15초동안 못움임직이게 함
        bMoveAble = false;
        GetWorld()->GetTimerManager().SetTimer(LeftRightTimerHandle, FTimerDelegate::CreateLambda(
            [this]()
            {
                bMoveAble = true;
            }
        ),0.15f , false);
    }
}

// 아래 방향키 입력
void ACameraPawn::Down(float axis)
{
    // 아래키를 입력 가능하다면
    if(bDownAble)
    {
        // 아래로 한칸 이동
        if (axis == 1)
        {
            TGameMode->MoveBlock(DIRECTION::DOWN);
        }
        else return;

        // 0.15초동안 아래로 못움직이게 함
        bDownAble = false;
        GetWorld()->GetTimerManager().SetTimer(DownTimerHandle, FTimerDelegate::CreateLambda(
            [this]()
            {
                bDownAble = true;
            }
        ),0.15f , false);
    }
}

// Drop키 입력
void ACameraPawn::Drop()
{
    // 테트리스 블록을 가능한 맨 아래로 내림
    TGameMode->DropBlock();
}

// 시계 방향 회전 키 입력
void ACameraPawn::RotateRight()
{
    // 시계 방향으로 블록을 회전
    TGameMode->RotateBlock(true);
}

// 반시계 방향 회전 키 입력
void ACameraPawn::RotateLeft()
{
    // 반시계 방향으로 블록을 회전
    TGameMode->RotateBlock(false);
}