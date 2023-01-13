// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisController.h"
#include "TetrisHud.h"

ATetrisController::ATetrisController()
{
	// ����ȭ�� ���� �������Ʈ�� ������
	static ConstructorHelpers::FClassFinder<UUserWidget> MAIN_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameStartWidget.GameStartWidget_C'"));
	if(MAIN_C.Succeeded())
	{
		MainWidgetClass = MAIN_C.Class;
	}

	// ���� ������ �����ϴ� ���� �������Ʈ�� ������
	static ConstructorHelpers::FClassFinder<UUserWidget> OVER_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameOverWidget.GameOverWidget_C'"));
	if(OVER_C.Succeeded())
	{
		OverWidgetClass = OVER_C.Class;
	}
	
	// ���� �ʱ�ȭ
	IsReady = false;

	// ������� ���콺�� ���̵��� ����
	SetShowMouseCursor(true);

}
void ATetrisController::BeginPlay()
{
	Super::BeginPlay();
	ReadyToWidget();
	ShowMainWidget();
}

// ���� �Ҵ�
void ATetrisController::ReadyToWidget()
{
	MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
	GameOverWidget = CreateWidget<UUserWidget>(this, OverWidgetClass);
}

// ���� �޴� ���� ǥ��
void ATetrisController::ShowMainWidget()
{
	SetInputMode(UIMode);
	
	if(!MainWidget->IsInViewport())
		MainWidget->AddToViewport();
}


// ���� ���� ���� ǥ��
void ATetrisController::ShowOverWidget()
{
	SetInputMode(UIMode);
	
	if(!GameOverWidget->IsInViewport())
		GameOverWidget->AddToViewport();
}

