// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisController.h"
#include "TetrisHud.h"

ATetrisController::ATetrisController()
{
	// 메인화면 위젯 블루프린트를 가져옴
	static ConstructorHelpers::FClassFinder<UUserWidget> MAIN_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameStartWidget.GameStartWidget_C'"));
	if(MAIN_C.Succeeded())
	{
		MainWidgetClass = MAIN_C.Class;
	}

	// 게임 오버시 등장하는 위젯 블루프린트를 가져옴
	static ConstructorHelpers::FClassFinder<UUserWidget> OVER_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameOverWidget.GameOverWidget_C'"));
	if(OVER_C.Succeeded())
	{
		OverWidgetClass = OVER_C.Class;
	}
	
	// 변수 초기화
	IsReady = false;

	// 사용자의 마우스가 보이도록 설정
	SetShowMouseCursor(true);

}
void ATetrisController::BeginPlay()
{
	Super::BeginPlay();
	ReadyToWidget();
	ShowMainWidget();
}

// 위젯 할당
void ATetrisController::ReadyToWidget()
{
	MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
	GameOverWidget = CreateWidget<UUserWidget>(this, OverWidgetClass);
}

// 메인 메뉴 위젯 표시
void ATetrisController::ShowMainWidget()
{
	SetInputMode(UIMode);
	
	if(!MainWidget->IsInViewport())
		MainWidget->AddToViewport();
}


// 게임 오버 위젯 표시
void ATetrisController::ShowOverWidget()
{
	SetInputMode(UIMode);
	
	if(!GameOverWidget->IsInViewport())
		GameOverWidget->AddToViewport();
}

