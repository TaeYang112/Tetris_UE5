// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "TetrisGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    RootComponent = Camera;
    bLeftRight = true;
    bDown = true;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
    TGameMode = Cast<ATetrisGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    

    PlayerInputComponent->BindAxis(TEXT("Down"), this, &ACameraPawn::Down);
    PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &ACameraPawn::Drop);
    PlayerInputComponent->BindAction(TEXT("ShowMap"), EInputEvent::IE_Pressed, this, &ACameraPawn::ShowMap);
    PlayerInputComponent->BindAction(TEXT("RotateLeft"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateLeft);
    PlayerInputComponent->BindAction(TEXT("RotateRight"), EInputEvent::IE_Pressed, this, &ACameraPawn::RotateRight);
    PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ACameraPawn::LeftRight);
}



void ACameraPawn::LeftRight(float axis)
{
    if(bLeftRight)
    {
        if(axis == 1) TGameMode->MoveBlock(DIRECTION::LEFT);
        else if(axis == -1) TGameMode->MoveBlock(DIRECTION::RIGHT);
        else return;

        bLeftRight = false;
        GetWorld()->GetTimerManager().SetTimer(LeftRightTimerHandle, FTimerDelegate::CreateLambda([this](){bLeftRight = true;}),0.15f , false);
    }
}


void ACameraPawn::Down(float axis)
{
    if(bDown)
    {
        if(axis == 1) TGameMode->MoveBlock(DIRECTION::DOWN);
        else return;

        bDown = false;
        GetWorld()->GetTimerManager().SetTimer(DownTimerHandle, FTimerDelegate::CreateLambda([this](){bDown = true;}),0.15f , false);
    }
}

void ACameraPawn::Drop()
{
    TGameMode->DropBlock();
}

void ACameraPawn::ShowMap()
{   
    TGameMode->ShowMap();
}
void ACameraPawn::RotateRight()
{
    TGameMode->RotateBlock(true);
}
void ACameraPawn::RotateLeft()
{
    TGameMode->RotateBlock(false);
}