#include "ScoreBoardWidget.h"
#include "TetrisGameState.h"
#include "TetrisSaveGame.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UScoreBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(GetWorld()));
	UTetrisSaveGame* SaveGameInstance = Cast<UTetrisSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveData"), 0));
	TArray<int32> ScoreBoard = SaveGameInstance->ScoreBoard;

	// 점수 정렬
	ScoreBoard.Sort([](const int32 &A, const int32 &B)
                                { return A > B;});

	// 텍스트 설정
	if(ScoreBoard.IsValidIndex(0))
		FirstScore->SetText(FText::FromString(FString::FromInt(ScoreBoard[0])));
	else
		FirstScore->SetText(FText::FromString(TEXT("0")));

	if(ScoreBoard.IsValidIndex(1))
		SecondScore->SetText(FText::FromString(FString::FromInt(ScoreBoard[1])));
	else
		SecondScore->SetText(FText::FromString(TEXT("0")));

	if(ScoreBoard.IsValidIndex(2))
		ThirdScore->SetText(FText::FromString(FString::FromInt(ScoreBoard[2])));
	else
		ThirdScore->SetText(FText::FromString(TEXT("0")));

	if(ScoreBoard.IsValidIndex(3))
		FourthScore->SetText(FText::FromString(FString::FromInt(ScoreBoard[3])));
	else
		FourthScore->SetText(FText::FromString(TEXT("0")));

	if(ScoreBoard.IsValidIndex(4))
		FifthScore->SetText(FText::FromString(FString::FromInt(ScoreBoard[4])));
	else
		FifthScore->SetText(FText::FromString(TEXT("0")));
}

void UScoreBoardWidget::UpdateWidget()
{
	
}



