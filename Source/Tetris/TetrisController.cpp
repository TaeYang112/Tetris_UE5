// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisController.h"
#include "TetrisHud.h"

ATetrisController::ATetrisController()
{
	
	ConstructorHelpers::FClassFinder<UTetrisHud> HUD_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/HudWidget.HudWidget_C'"));
	if(HUD_C.Succeeded())
	{
		HudClass = HUD_C.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> MAIN_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameStartWidget.GameStartWidget_C'"));
	if(MAIN_C.Succeeded())
	{
		MainWidgetClass = MAIN_C.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> OVER_C(TEXT("WidgetBlueprint'/Game/Tetris/BluePrint/GameOverWidget.GameOverWidget_C'"));
	if(OVER_C.Succeeded())
	{
		OverWidgetClass = OVER_C.Class;
	}
	
	IsReady = false;
	SetShowMouseCursor(true);

}
void ATetrisController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATetrisController::ReadyToWidget()
{
	IsReady = true;
	MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
	HUDWidget = CreateWidget<UTetrisHud>(this, HudClass);
	GameOverWidget = CreateWidget<UUserWidget>(this, OverWidgetClass);
}
void ATetrisController::ShowMainWidget()
{
	if(IsReady == false) ReadyToWidget();
	
	SetInputMode(UIMode);
	if(!MainWidget->IsInViewport()) MainWidget->AddToViewport();
}


void ATetrisController::GameStart()
{
	if(IsReady == false) ReadyToWidget();
	
	if(GameOverWidget->IsInViewport()) GameOverWidget->RemoveFromViewport();
	if(MainWidget->IsInViewport()) MainWidget->RemoveFromViewport();
	
	SetInputMode(InputMode);
	if(!HUDWidget->IsInViewport()) HUDWidget->AddToViewport();
}

void ATetrisController::ShowOverWidget()
{
	SetInputMode(UIMode);
	if(!GameOverWidget->IsInViewport()) GameOverWidget->AddToViewport();
}

