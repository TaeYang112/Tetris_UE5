// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisGameState.h"

#include "TetrisSaveGame.h"
#include "Kismet/GameplayStatics.h"

ATetrisGameState::ATetrisGameState()
{
	Score = 0;
	//SaveGameInstance = Cast<UTetrisSaveGame>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGame::StaticClass()));
	SaveGameInstance = Cast<UTetrisSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveData"), 0));
	if (nullptr == SaveGameInstance)
	{
		SaveGameInstance = GetMutableDefault<UTetrisSaveGame>();
	}
}

void ATetrisGameState::BeginPlay()
{
	Super::BeginPlay();
	FOnScoreChanged.Broadcast();
}

int32 ATetrisGameState::GetScore() const
{
	return Score;
}

void ATetrisGameState::AddScore(const int32 NewScore)
{
	Score += NewScore;
	FOnScoreChanged.Broadcast();
	
}
void ATetrisGameState::SaveScore()
{
	SaveGameInstance->ScoreBoard.Add(Score);
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveData"), 0);
}



void ATetrisGameState::ClearScore()
{
	Score = 0;
	FOnScoreChanged.Broadcast();
}
