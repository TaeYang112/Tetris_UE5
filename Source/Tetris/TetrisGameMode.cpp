// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisGameMode.h"
#include "TetrisController.h"
#include "CameraPawn.h"
#include "TetrisSaveGame.h"
#include "Algo/Copy.h"
#include "Kismet/GameplayStatics.h"


ATetrisGameMode::ATetrisGameMode()
{
	CurrentLoc.X = 0;
	CurrentLoc.Y = 0;
	CurrentRotation = 0;
	LastTime = 0;
	IsGamePlaying = false;
	CurrentType = BlockType::I;
	GameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(this));


	PlayerControllerClass = ATetrisController::StaticClass();
	DefaultPawnClass = ACameraPawn::StaticClass();
	GameStateClass = ATetrisGameState::StaticClass();
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly InputMode;
	PlayerController = Cast<ATetrisController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if(PlayerController)
	{
		PlayerController->ShowMainWidget();
	}
}

void ATetrisGameMode::StartGame()
{
	IsGamePlaying = true;
	if(PlayerController) PlayerController->GameStart();
	GetWorld()->GetTimerManager().SetTimer(SpawnBlockTimerHandle, FTimerDelegate::CreateUObject(this, &ATetrisGameMode::LoopFalling),1.5f , false);
	GetWorld()->GetTimerManager().SetTimer(LastTimeTimerHandle, FTimerDelegate::CreateLambda([this]()->void{LastTime++;}),1.0f , true);
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i] = GetWorld()->SpawnActor<ACube>(ACube::StaticClass(),FVector(-500.0f,-500.0f,-500.0f),FRotator::ZeroRotator);
	}
	for(int32 i=0; i<4;i++)
	{
		NextBlock[i] = nullptr;
	}
	SummonBlock();
}

void ATetrisGameMode::GameOver()
{
	GetGameState<ATetrisGameState>()->SaveScore();
	IsGamePlaying = false;
	GetWorld()->GetTimerManager().ClearTimer(SpawnBlockTimerHandle);
	if(PlayerController != nullptr) PlayerController->ShowOverWidget();
}

void ATetrisGameMode::ReStart()
{
	memset(TetrisMap, 0, sizeof(TetrisMap));
	for(int32 i=0; i<4; i++)
	{
		if( MovingBlockActor[i] != nullptr ) MovingBlockActor[i]->Destroy();
		if( InvBlockActor[i] != nullptr ) InvBlockActor[i]->Destroy();
		if( NextBlock[i] != nullptr ) NextBlock[i]->Destroy();
	}

	for(int32 i=0; i<BlockActors.Num(); i++)
	{
		if( BlockActors[i] != nullptr ) BlockActors[i]->Destroy();
	}
	LastTime = 0;
	GetGameState<ATetrisGameState>()->ClearScore();
	StartGame();
}

void ATetrisGameMode::MkNextBlock()
{
	NextType = static_cast<BlockType>(FMath::RandRange(BlockType::I, BlockType::T));
	for (int32 i = 0; i < 4; i++)
	{
		int32 dx = GetBlockTransform(NextType,0)[i].X;
		int32 dy = GetBlockTransform(NextType,0)[i].Y;
		FVector SpawnLocation = FVector::ZeroVector;
		if(NextType == BlockType::I) SpawnLocation = FVector(750.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);
		else if(NextType == BlockType::O) SpawnLocation = FVector(750.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);
		else SpawnLocation = FVector(830.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);
		
		NextBlock[i] = GetWorld()->SpawnActor<ACube>(ACube::StaticClass(),SpawnLocation,FRotator::ZeroRotator);
		NextBlock[i]->setColor(NextType,1.0f);
	}
}
void ATetrisGameMode::SummonBlock()
{
	if(NextBlock[0] == nullptr) MkNextBlock();
	for(int32 i=0; i<4; i++) MovingBlockActor[i] = nullptr;
	CurrentLoc.X = 4;
	CurrentLoc.Y = 2;
	CurrentType = NextType;
	CurrentRotation = 0;
	for(int32 i = 0; i < 4; i++)
	{

		int32 dx = CurrentLoc.X + GetBlockTransform(CurrentType,CurrentRotation)[i].X;
		int32 dy = CurrentLoc.Y + GetBlockTransform(CurrentType,CurrentRotation)[i].Y;
		FPoint TempPoint = {dx,dy};
		if(!IsLocationPossible(TempPoint))
		{
			GameOver();
			return;
		}
	
	}
	for(int32 i = 0; i < 4; i++)
	{
		int32 dx = GetBlockTransform(CurrentType,CurrentRotation)[i].X;
		int32 dy = GetBlockTransform(CurrentType,CurrentRotation)[i].Y;
		FVector  SpawnLocation = FVector(START_LOC_X + BLOCK_SIZE * (dx + CurrentLoc.X),START_LOC_Y + BLOCK_SIZE * (dy + CurrentLoc.Y),100.0f);
		MovingBlock[i].X = CurrentLoc.X + dx;
		MovingBlock[i].Y = CurrentLoc.Y + dy;
		
		MovingBlockActor[i] = NextBlock[i];
		MovingBlockActor[i]->SetActorLocation(SpawnLocation);
		NextBlock[i] = nullptr;
		InvBlockActor[i]->setColor(CurrentType,0.5f);
		
	}
	MkNextBlock();
	SyncInvBlock();
}

void ATetrisGameMode::LoopFalling()
{
	if(IsGamePlaying == false) return;
	float Rate = FMath::Clamp(1.5f - LastTime*0.005f,0.1f,3.0f);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f"),Rate));
	MoveBlock(DIRECTION::DOWN);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnBlockTimerHandle, FTimerDelegate::CreateUObject(this, &ATetrisGameMode::LoopFalling),Rate , false);
}




void ATetrisGameMode::SyncInvBlock()
{
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i]->SetActorLocation( MovingBlockActor[i]->GetActorLocation() );
	}
	int32 dy = 0;
	bool LoopEnd = false;
	while(!LoopEnd)
	{
		dy++;
		for(int32 i = 0; i<4; i++)
		{
			
			int32 tmp_x = MovingBlock[i].X;
			int32 tmp_y = MovingBlock[i].Y + dy;
			if(tmp_y >= HEIGHT || tmp_y<0)
			{
				dy--;
				LoopEnd = true;
				break;
			}
			else if(TetrisMap[tmp_y][tmp_x] == 1)
			{ 
				dy--;
				LoopEnd = true;
				break;
			}
		}
		
	}
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i]->AddActorLocalOffset(FVector(0.0f,dy*BLOCK_SIZE,0.0f));
	}
	
}
void ATetrisGameMode::DropBlock()
{
	if(MovingBlockActor[0] == nullptr) return;
	int32 dy = 0;
	bool LoopEnd = false;
	
	while(!LoopEnd)
	{
		dy++;
		for(int32 i = 0; i<4; i++)
		{
			
			int32 tmp_x = MovingBlock[i].X;
			int32 tmp_y = MovingBlock[i].Y + dy;
			if(tmp_y >= HEIGHT || tmp_y<0)
			{
				dy--;
				LoopEnd = true;
				break;
			}
			else if(TetrisMap[tmp_y][tmp_x] == 1)
			{ 
				dy--;
				LoopEnd = true;
				break;
			}
		}
		
	}
	for(int32 i = 0; i<4; i++)
	{
		MovingBlock[i].Y += dy;
		MovingBlockActor[i]->AddActorLocalOffset(FVector(0.0f,dy*BLOCK_SIZE,0.0f));
	}
	EndMoving();
	
}

bool ATetrisGameMode::MoveBlock(DIRECTION dir, FPoint *movBlock)
{
	if(MovingBlockActor[0] == nullptr) return false;
	int32 dx = 0;
	int32 dy = 0;
	if(movBlock == nullptr) movBlock = MovingBlock;
	switch(dir)
	{
		case DIRECTION::LEFT:
			dx = -1;
			break;
		case DIRECTION::RIGHT:
			dx = 1;
			break;
		case DIRECTION::DOWN:
			dy = 1;
			break;
		default:
			break;
	}
	
	for(int32 i = 0; i<4; i++)
	{
		int32 tmp_x = movBlock[i].X + dx;
		int32 tmp_y = movBlock[i].Y + dy;
		FPoint TempPoint = {tmp_x,tmp_y};
		if(!IsLocationPossible(TempPoint))
		{
			if(dir == DIRECTION::DOWN) EndMoving();
			return false;
		}
		
	}

	 // IS NOT TEST
	{
		for(int32 i=0; i<4; i++)
		{
			movBlock[i].X += dx;
			movBlock[i].Y += dy;
		if(movBlock == MovingBlock)	MovingBlockActor[i]->AddActorLocalOffset(FVector(BLOCK_SIZE * dx,BLOCK_SIZE * dy,0.0f));
		}
		CurrentLoc.X += dx;
		CurrentLoc.Y += dy;
	}
	SyncInvBlock();
	return true;
}

void ATetrisGameMode::EndMoving()
{
	
	for(int32 i=0; i<4; i++)
	{
		int32 tmp_x = MovingBlock[i].X;
		int32 tmp_y = MovingBlock[i].Y;
			
		TetrisMap[tmp_y][tmp_x] = 1;
		BlockActors.Add(MovingBlockActor[i]);
	}
	CheckRows();
	SummonBlock();
}

void ATetrisGameMode::CheckRows()
{
	int32 Combo = 0;
	TArray<int32> Rows_Y;
	for(int32 i=0; i<4; i++)
	{
		Rows_Y.Add(MovingBlock[i].Y) ;
	}
	Rows_Y.Sort([](const int32& A, const int32& B) {
		return A < B;
	});
	for(int32 i=0; i<4; i++)
	{
		int32 count = 0;
		int32 tmp_y = Rows_Y[i];
		for(int32 j=0; j<WIDTH;j++)
		{
			
			if(TetrisMap[tmp_y][j] == 0) break;
			count++;
		}
		if(count == 10)
		{
			Combo++;
			BreakingRow(tmp_y);
			for(int32 j = 0; j <Rows_Y.Num();j++)
			{
				if(Rows_Y[i]<tmp_y) Rows_Y[j]++;
			}
		}
	}
	switch(Combo)
	{
	case 1: GetGameState<ATetrisGameState>()->AddScore(100);
		break;
	case 2: GetGameState<ATetrisGameState>()->AddScore(300);
		break;
	case 3: GetGameState<ATetrisGameState>()->AddScore(1000);
		break;
	case 4: GetGameState<ATetrisGameState>()->AddScore(2000);
		break;
	default:
		break;
			
	}
}

void ATetrisGameMode::BreakingRow(int32 Row)
{
	if(MovingBlockActor[0] == nullptr) return;
	for(int32 j=0; j<WIDTH; j++)
	{
		TetrisMap[Row][j] = 0;
	}
	int32 i = 0; 
	while(i<BlockActors.Num())
	{
		if(BlockActors.IsValidIndex(i) && BlockActors[i] != nullptr)
		{
			if(BlockActors[i]->GetActorLocation().Y == START_LOC_Y + Row * BLOCK_SIZE)
			{
				BlockActors[i]->Destroy();
				BlockActors.RemoveAt(i);
				i--;
			}
		}
		i++;
	}
	
	PullDown(Row);
}

void ATetrisGameMode::PullDown(int32 StartRow)
{
	if(MovingBlockActor[0] == nullptr) return;
	for(int32 i = StartRow; i>0; i--)
	{
		for(int32 j = 0; j<WIDTH; j++)
		{
			TetrisMap[i][j] = TetrisMap[i-1][j];
		}
		
	}
	int32 StartRowOfActor = START_LOC_Y + StartRow * BLOCK_SIZE;
	for(int32 i = 0; i<BlockActors.Num();i++)
	{
		if(BlockActors[i] != nullptr)
		{
			if(BlockActors[i]->GetActorLocation().Y < StartRowOfActor)
				BlockActors[i]->AddActorLocalOffset(FVector(0.0f,BLOCK_SIZE,0.0f));
		}
	}
	for(int32 j=0; j<WIDTH; j++)
		TetrisMap[0][j] = 0;
		
}

void ATetrisGameMode::RotateBlock(bool bClockwise)
{
	if(MovingBlockActor[0] == nullptr) return;
	int32 tempRotation = CurrentRotation;
	bool bPossible = true;
	FPoint TempPoint[4];
	if(bClockwise)
		tempRotation = (tempRotation + 1) % 4;
	else
	{
		if(tempRotation == 0) tempRotation = 3;
		else
			tempRotation -= 1;
	}

	// Rotate Test
	for (int i = 0; i < 4; i++)
	{
		TempPoint[i].X = CurrentLoc.X + GetBlockTransform(CurrentType,tempRotation)[i].X;
		TempPoint[i].Y = CurrentLoc.Y + GetBlockTransform(CurrentType,tempRotation)[i].Y;
		if(IsLocationPossible(TempPoint[i]) == false) bPossible = false;
	}
	

	if(bPossible == false)
	{
		//Try Move
		if(MoveBlock(DIRECTION::LEFT,TempPoint) || MoveBlock(DIRECTION::RIGHT,TempPoint))
		{
			bPossible = true;
		}
		else bPossible = false;
	}

	if(bPossible == true)
	{
		for(int i=0; i<4; i++)
		{
		
			MovingBlock[i].X =  TempPoint[i].X;
			MovingBlock[i].Y =  TempPoint[i].Y;
		
			int32 BlockX = START_LOC_X + BLOCK_SIZE * TempPoint[i].X;
			int32 BlockY = START_LOC_Y + BLOCK_SIZE * TempPoint[i].Y;
			MovingBlockActor[i]->SetActorLocation(FVector(BlockX,BlockY,100.0f));

			CurrentRotation = tempRotation;
		
		}
	}
	SyncInvBlock();
}

const FPoint* ATetrisGameMode::GetBlockTransform(BlockType type, int32 Rotation = 0)
{
	static FPoint IBlock[4][4] = 
	{	{ {0,-1},	{1,-1},	{2,-1},		{-1,-1} },
		{ {1,-1},	{1,0},	{1,1},		{1,-2} },
		{ {1,0},	{0,0},	{-1,0},		{2,0} },
		{ {0,0},	{0,-1},	{0,-2},		{0,1} } };
	static FPoint OBlock[4][4] =
	{	{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} } };
	static FPoint SBlock[4][4] =
	{	{ {0,-1},	{-1,-1},{0,-2},		{1,-2} },
		{ {0,-1},	{0,-2},	{1,-1},		{1,0} },
		{ {0,-1},	{1,-1},	{0,0},		{-1,0} },
		{ {0,-1},	{0,0},	{-1,-1},	{-1,-2} } };
	static FPoint ZBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {0,-2},		{-1,-2} },
		{ {0,-1},	{0,0},	{1,-1},		{1,-2} },
		{ {0,-1},	{-1,-1},{0,0},		{1,0} },
		{ {0,-1},	{0,-2}, {-1,-1},	{-1,0} } };
	static FPoint JBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {-1,-1},	{-1,-2} },
		{ {0,-1},	{0,0},	{0,-2},		{1,-2} },
		{ {0,-1},	{-1,-1},{1,-1},		{1,0} },
		{ {0,-1},	{0,-2}, {0,0},		{-1,0} } };
	static FPoint LBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {-1,-1},	{1,-2} },
		{ {0,-1},	{0,0},	{0,-2},		{1,0} },
		{ {0,-1},	{-1,-1},{1,-1},		{-1,0} },
		{ {0,-1},	{0,-2}, {0,0},		{-1,-2} } };
	static FPoint TBlock[4][4] =
	{	{ {0,-1},	{1,-1},{-1,-1},	{0,-2} },
		{ {0,-1},	{0,-2},	{0,0},		{1,-1} },
		{ {0,-1},	{1,-1}, {-1,-1},	{0,0} },
		{ {0,-1},	{0,0},	{0,-2},		{-1,-1} } };

	switch(type)
	{
		case BlockType::I:
			return IBlock[Rotation];
		case BlockType::O:
			return OBlock[Rotation];
		case BlockType::S:
			return SBlock[Rotation];
		case BlockType::Z:
			return ZBlock[Rotation];
		case BlockType::J:
			return JBlock[Rotation];
		case BlockType::L:
			return LBlock[Rotation];
		case BlockType::T:
			return TBlock[Rotation];
		default :
			return IBlock[Rotation];
	}
}

bool ATetrisGameMode::IsLocationPossible(FPoint location)
{
	int32 tmp_x = location.X;
	int32 tmp_y = location.Y;
	if(tmp_x >= WIDTH || tmp_y >= HEIGHT || tmp_x<0 || tmp_y<0)
	{
		return false;
	}
	else if(TetrisMap[tmp_y][tmp_x] == 1 )
	{
		return false;
	}
	else return true;
}


void ATetrisGameMode::ShowMap()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f"),LastTime));
	
	/*
	int32 TMap[HEIGHT][WIDTH];
	memcpy(TMap,TetrisMap,sizeof(TetrisMap));
	for(int32 i=0;i<4;i++)
	{
		int32 tmp_x = MovingBlock[i].X;
		int32 tmp_y = MovingBlock[i].Y;
		TMap[tmp_y][tmp_x] = 1;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("-----------------------------")));
	for(int32 i=HEIGHT-1;i>=0;i--)
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%d %d %d %d %d %d %d %d %d %d"), TMap[i][0],TMap[i][1],TMap[i][2],TMap[i][3],TMap[i][4],TMap[i][5],TMap[i][6],TMap[i][7],TMap[i][8],TMap[i][9]));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("-----------------------------")));
	*/
}


