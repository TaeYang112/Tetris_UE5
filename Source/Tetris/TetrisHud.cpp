// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisHud.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTetrisHud::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->FOnScoreChanged.AddUObject(this,&UTetrisHud::UpdateWidget);

}


void UTetrisHud::UpdateWidget()
{
	FString ScoreString = "SCORE : ";
	ScoreString += FString::FromInt(GameState->GetScore());
	FText ScoreText = FText::FromString(ScoreString);
	CurrentScore->SetText(ScoreText);
}
