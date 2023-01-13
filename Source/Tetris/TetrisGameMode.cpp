// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisGameMode.h"
#include "TetrisController.h"
#include "CameraPawn.h"
#include "TetrisSaveGame.h"
#include "Algo/Copy.h"
#include "Kismet/GameplayStatics.h"


ATetrisGameMode::ATetrisGameMode()
{
	// 변수 초기화
	CurrentLoc.X = 0;
	CurrentLoc.Y = 0;
	CurrentRotation = 0;
	LastTime = 0;
	IsGamePlaying = false;
	CurrentType = BlockType::I;
	GameState = Cast<ATetrisGameState>(UGameplayStatics::GetGameState(this));

	// 게임의 기본 설정 변경
	PlayerControllerClass = ATetrisController::StaticClass();
	DefaultPawnClass = ACameraPawn::StaticClass();
	GameStateClass = ATetrisGameState::StaticClass();
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// 게임 시작
void ATetrisGameMode::StartGame()
{
	
	IsGamePlaying = true;

	PlayerController = Cast<ATetrisController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	// 블록 소환 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(SpawnBlockTimerHandle, FTimerDelegate::CreateUObject(this, &ATetrisGameMode::LoopFalling),1.5f , false);

	// 게임 시간 측정 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(LastTimeTimerHandle, FTimerDelegate::CreateLambda([this]()->void{LastTime++;}),1.0f , true);

	// 투명 미리보기 블록 할당
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i] = GetWorld()->SpawnActor<ACube>(ACube::StaticClass(),FVector(-500.0f,-500.0f,-500.0f),FRotator::ZeroRotator);
	}

	// 블록 소환
	SummonBlock();
}

// 게임 오버
void ATetrisGameMode::GameOver()
{
	// 현재 점수를 저장함
	GetGameState<ATetrisGameState>()->SaveScore();
	
	IsGamePlaying = false;

	// 타이머 종료
	GetWorld()->GetTimerManager().ClearTimer(SpawnBlockTimerHandle);
	// 게임 오버 위젯 표시
	if(PlayerController != nullptr)
	{
		PlayerController->ShowOverWidget();
	}
}

// 재시작
void ATetrisGameMode::ReStart()
{
	// 테트리스 맵 초기화
	memset(TetrisMap, 0, sizeof(TetrisMap));

	// 전에 할당한 액터 제거
	for(int32 i=0; i<4; i++)
	{
		if( MovingBlockActor[i] != nullptr ) MovingBlockActor[i]->Destroy();
		if( InvBlockActor[i] != nullptr ) InvBlockActor[i]->Destroy();
		if( NextBlock[i] != nullptr )
		{
			NextBlock[i]->Destroy();
			NextBlock[i] = nullptr;
		}
	}
	for(int32 i=0; i<BlockActors.Num(); i++)
	{
		if( BlockActors[i] != nullptr ) BlockActors[i]->Destroy();
	}

	// 시간 및 점수 초기화
	LastTime = 0;
	GetGameState<ATetrisGameState>()->ClearScore();

	// 게임 시작
	StartGame();
}

// 다음 소환될 블럭을 생성
void ATetrisGameMode::MakeNextBlock()
{
	// 랜덤으로 테트리스 블록 타입을 결정
	NextType = static_cast<BlockType>(FMath::RandRange(BlockType::I, BlockType::T));
	
	for (int32 i = 0; i < 4; i++)
	{
		// 해당 타입에 맞는 좌표를 가져옴
		int32 dx = GetBlockTransform(NextType,0)[i].X;
		int32 dy = GetBlockTransform(NextType,0)[i].Y;
		FVector SpawnLocation = FVector::ZeroVector;

		// 다음 소환될 블록을 보여줄 때 가운데에 배치되어 보여지게 함
		if(NextType == BlockType::I)
			SpawnLocation = FVector(750.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);
		else if(NextType == BlockType::O)
			SpawnLocation = FVector(750.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);
		else
			SpawnLocation = FVector(830.0f + BLOCK_SIZE * dx,-290 + BLOCK_SIZE * dy,100.0f);

		// 블록 생성 및 좌표 설정
		NextBlock[i] = GetWorld()->SpawnActor<ACube>(ACube::StaticClass(),SpawnLocation,FRotator::ZeroRotator);

		// 타입에 맞는 색상으로 변경
		NextBlock[i]->setColor(NextType,1.0f);
	}
}

// 다음에 생성될 블록을 현재블록으로 가져옴
void ATetrisGameMode::SummonBlock()
{
	// 다음에 생성될 블록이 없으면 생성함
	if(NextBlock[0] == nullptr)
	{
		MakeNextBlock();
	}
	CurrentLoc.X = 4;
	CurrentLoc.Y = 2;

	// 다음 타입을 현재 타입으로 가져옴
	CurrentType = NextType;

	// 회전값을 0으로 설정
	CurrentRotation = 0;

	// 블록이 소환될 자리에 배치가 가능한지 검사. 불가능시 게임 오버
	for(int32 i = 0; i < 4; i++)
	{
		int32 dx = CurrentLoc.X + GetBlockTransform(CurrentType,CurrentRotation)[i].X;
		int32 dy = CurrentLoc.Y + GetBlockTransform(CurrentType,CurrentRotation)[i].Y;
		Point TempPoint = {dx,dy};
		if(!IsLocationPossible(TempPoint))
		{
			GameOver();
			return;
		}
	}

	// 소환될 자리에 실제로 소환
	for(int32 i = 0; i < 4; i++)
	{
		// 테트리스 블록을 구성하는 각각의 큐브가 어디에 위치해야 하는지 가져옴
		int32 dx = GetBlockTransform(CurrentType,CurrentRotation)[i].X;
		int32 dy = GetBlockTransform(CurrentType,CurrentRotation)[i].Y;

		// 액터가 배치될 자리 설정
		FVector  SpawnLocation = FVector(START_LOC_X + BLOCK_SIZE * (dx + CurrentLoc.X),START_LOC_Y + BLOCK_SIZE * (dy + CurrentLoc.Y),100.0f);

		// 블록 위치 설정
		MovingBlock[i].X = CurrentLoc.X + dx;
		MovingBlock[i].Y = CurrentLoc.Y + dy;

		// 다음에 소환할 블록을 가져와서 사용함
		MovingBlockActor[i] = NextBlock[i];
		MovingBlockActor[i]->SetActorLocation(SpawnLocation);
		NextBlock[i] = nullptr;

		// 투명 미리보기 블럭을 현재 타입에 맞게 색상 변경
		InvBlockActor[i]->setColor(CurrentType,0.5f);
		
	}

	// 다음에 소환될 블록을 생성함
	MakeNextBlock();

	// 투명 미리보기 블록의 위치를 소환된 블록과 동기화
	SyncInvBlock();
}

// 타이머에 의해 반복 호출되는 함수. 블록이 밑으로 내려감
void ATetrisGameMode::LoopFalling()
{
	if(IsGamePlaying == false)
		return;

	// 떨어지는 간격. 현재까지 진행된 게임 시간에 따라 간격이 줄어듬
	float Rate = FMath::Clamp(1.5f - LastTime*0.005f,0.1f,3.0f);

	// 블록을 밑으로 내림
	MoveBlock(DIRECTION::DOWN);

	// 다시 이 함수를 호출함
	GetWorld()->GetTimerManager().SetTimer(SpawnBlockTimerHandle, FTimerDelegate::CreateUObject(this, &ATetrisGameMode::LoopFalling),Rate , false);
}



// 투명 블록과 떨어지는 블록의 위치를 맞춰줌
void ATetrisGameMode::SyncInvBlock()
{
	// 좌표를 완전히 같게 설정함
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i]->SetActorLocation( MovingBlockActor[i]->GetActorLocation() );
	}
	int32 dy = 0;
	bool LoopEnd = false;
	while(!LoopEnd)
	{
		dy++;
		
		// 밑으로 한칸씩 내리면서 막힐 때 까지 검사함
		// 테트리스 블록을 구성하는 블록 하나라도 막힌다면 멈춤
		for(int32 i = 0; i<4; i++)
		{
			int32 tmp_x = MovingBlock[i].X;
			int32 tmp_y = MovingBlock[i].Y + dy;

			// 맵을 벗어나거나 다른 블록에 의해 막힘
			if(tmp_y >= HEIGHT || tmp_y<0 || TetrisMap[tmp_y][tmp_x] == 1)
			{
				// y를 막현던 좌표 바로 위로 되돌림
				dy--;
				
				LoopEnd = true;
				break;
			}
		}
	}
	// 위에서 찾은 좌표를 액터의 좌표로 설정함
	for(int32 i = 0; i<4; i++)
	{
		InvBlockActor[i]->AddActorLocalOffset(FVector(0.0f,dy*BLOCK_SIZE,0.0f));
	}
	
}

// 블록을 가능한 맨 아래로 내리는 함수
void ATetrisGameMode::DropBlock()
{
	if(MovingBlockActor[0] == nullptr)
		return;
	
	int32 dy = 0;
	bool LoopEnd = false;
	
	while(!LoopEnd)
	{
		dy++;

		// 밑으로 한칸씩 내리면서 막힐 때 까지 검사함
		// 테트리스 블록을 구성하는 블록 하나라도 막힌다면 멈춤
		for(int32 i = 0; i<4; i++)
		{
			
			int32 tmp_x = MovingBlock[i].X;
			int32 tmp_y = MovingBlock[i].Y + dy;
			
			// 맵을 벗어나거나 다른 블록에 의해 막힘
			if(tmp_y >= HEIGHT || tmp_y<0 || TetrisMap[tmp_y][tmp_x] == 1)
			{
				// y를 막현던 좌표 바로 위로 되돌림
				dy--;
				
				LoopEnd = true;
				break;
			}
		}
		
	}

	// 위에서 찾은 좌표를 액터의 좌표로 설정함
	for(int32 i = 0; i<4; i++)
	{
		MovingBlock[i].Y += dy;
		MovingBlockActor[i]->AddActorLocalOffset(FVector(0.0f,dy*BLOCK_SIZE,0.0f));
	}

	// 아래로 더이상 내려갈 수 없기 때문에 현재 블록의 이동을 끝내고 다음 블록 소환
	EndMoving();
}

// 블록을 매개변수로 들어온 방향을 향해 움직임
bool ATetrisGameMode::MoveBlock(DIRECTION dir, Point *movBlock)
{
	if(MovingBlockActor[0] == nullptr)
		return false;
	
	int32 dx = 0;
	int32 dy = 0;
	
	if(movBlock == nullptr)
		movBlock = MovingBlock;
	
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

	// 해당 방향으로 움직일 수 있는지 테스트
	for(int32 i = 0; i<4; i++)
	{
		int32 tmp_x = movBlock[i].X + dx;
		int32 tmp_y = movBlock[i].Y + dy;
		Point TempPoint = {tmp_x,tmp_y};

		// 불가능하다면
		if(!IsLocationPossible(TempPoint))
		{
			// 만약 아래로 이동중이였다면 어딘가 위에 도착했다는 뜻이므로 이동 중단
			if(dir == DIRECTION::DOWN)
				EndMoving();

			// 아니라면 리턴
			return false;
		}
		
	}

	// 실제로 움직임
	{
		for(int32 i=0; i<4; i++)
		{
			movBlock[i].X += dx;
			movBlock[i].Y += dy;
			if(movBlock == MovingBlock)
			{
				// 블록 액터도 움직여줌
				MovingBlockActor[i]->AddActorLocalOffset(FVector(BLOCK_SIZE * dx,BLOCK_SIZE * dy,0.0f));
			}
		}
		CurrentLoc.X += dx;
		CurrentLoc.Y += dy;
	}

	// 투명블록도 움직여줌
	SyncInvBlock();
	return true;
}

// 블록이 이동중 어딘가 위에 도달했을 경우 이동을 멈추고 다른 블록 소환
void ATetrisGameMode::EndMoving()
{
	// 테트리스 맵 배열에 추가
	for(int32 i=0; i<4; i++)
	{
		int32 tmp_x = MovingBlock[i].X;
		int32 tmp_y = MovingBlock[i].Y;
			
		TetrisMap[tmp_y][tmp_x] = 1;
		BlockActors.Add(MovingBlockActor[i]);
	}

	// 가로줄이 맞춰졌는지 확인
	CheckRows();

	// 다음 블록 소환
	SummonBlock();
}

// 맞춰진 가로줄이 있는지 확인
void ATetrisGameMode::CheckRows()
{
	// 한번에 맞춰진 가로줄의 개수에 따라 점수를 더줌
	int32 Combo = 0;

	// 현재 움직이고 있는 블록이 있던 가로줄만 확인하면 되기 떄문에 배열로 저장
	TArray<int32> Rows_Y;
	for(int32 i=0; i<4; i++)
	{
		Rows_Y.Add(MovingBlock[i].Y) ;
	}

	// 배열 정렬
	Rows_Y.Sort([](const int32& A, const int32& B) {
		return A < B;
	});

	
	for(int32 i=0; i<4; i++)
	{
		int32 count = 0;
		int32 tmp_y = Rows_Y[i];

		// 해당 가로줄에 있는 블록의 개수를 셈
		for(int32 j=0; j<WIDTH;j++)
		{
			// 한개라도 빈공간이 발견되면 반복문 탈출
			if(TetrisMap[tmp_y][j] == 0)
				break;
			
			count++;
		}

		// 가로줄이 블록으로 꽉찼다면
		if(count == 10)
		{
			// 콤보 증가
			Combo++;

			// 가로줄에 있던 블록을 없앤 뒤, 위에 있는 모든 블록을 한칸씩 내림
			BreakingRow(tmp_y);

			// 한칸씩 내렸기 때문에 검사할 가로줄의 y도 한칸씩 내림
			for(int32 j = 0; j <Rows_Y.Num();j++)
			{
				if(Rows_Y[i]<tmp_y)
					Rows_Y[j]++;
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

// 가로 한줄을 없앰
void ATetrisGameMode::BreakingRow(int32 Row)
{
	if(MovingBlockActor[0] == nullptr)
		return;

	// 테트리스 맵에서 제거
	for(int32 j=0; j<WIDTH; j++)
	{
		TetrisMap[Row][j] = 0;
	}

	// 블록 액터 배열에서 제거
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

	// 없앤 줄 위에 있는 블록들을 밑으로 내림
	PullDown(Row);
}

// 매개변수로 받은 줄 위에 있는 블록들을 끌어낼미
void ATetrisGameMode::PullDown(int32 StartRow)
{
	if(MovingBlockActor[0] == nullptr)
		return;

	// 테트리스 맵
	for(int32 i = StartRow; i>0; i--)
	{
		for(int32 j = 0; j<WIDTH; j++)
		{
			TetrisMap[i][j] = TetrisMap[i-1][j];
		}
		
	}

	// 액터
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

// 테트리스 블록을 회전
void ATetrisGameMode::RotateBlock(bool bClockwise)
{
	if(MovingBlockActor[0] == nullptr) return;
	
	int32 tempRotation = CurrentRotation;
	bool bPossible = true;
	Point TempPoint[4];

	// 시계방향
	if(bClockwise)
	{
		// 회전 정도를 증가시킴
		tempRotation = (tempRotation + 1) % 4;
	}
	// 반시계 방향
	else
	{
		// 회전 정도를 감소시킴
		if(tempRotation == 0) tempRotation = 3;
		else
			tempRotation -= 1;
	}

	// 블록을 회전시켰을 때 막히지 않는지 테스트
	for (int i = 0; i < 4; i++)
	{
		TempPoint[i].X = CurrentLoc.X + GetBlockTransform(CurrentType,tempRotation)[i].X;
		TempPoint[i].Y = CurrentLoc.Y + GetBlockTransform(CurrentType,tempRotation)[i].Y;
		if(IsLocationPossible(TempPoint[i]) == false) bPossible = false;
	}
	

	// 회전이 불가능하다면
	if(bPossible == false)
	{
		// 블록을 왼쪽 혹은 오른쪽으로 이동했을때를 가정했을 때 배치가 가능하다면
		if(MoveBlock(DIRECTION::LEFT,TempPoint) || MoveBlock(DIRECTION::RIGHT,TempPoint))
		{
			// 가능하다고 플래그 변수 변경
			bPossible = true;
		}
	}

	// 회전이 가능하다면
	if(bPossible == true)
	{
		for(int i=0; i<4; i++)
		{
			// 2차원 배열상의 실제 블록 좌표 변경
			MovingBlock[i].X =  TempPoint[i].X;
			MovingBlock[i].Y =  TempPoint[i].Y;

			// 블록 액터의 좌표를 계산
			int32 BlockX = START_LOC_X + BLOCK_SIZE * TempPoint[i].X;
			int32 BlockY = START_LOC_Y + BLOCK_SIZE * TempPoint[i].Y;

			// 블록 액터의 좌표 변경
			MovingBlockActor[i]->SetActorLocation(FVector(BlockX,BlockY,100.0f));

			CurrentRotation = tempRotation;
		}

		// 투명한 미리보기 블록도 움직여줌
		SyncInvBlock();
	}
}

// 블록타입과 회전량에 따른 블록의 위치를 반환
// 기준점 ( CurrentLocation ) 에 대한 상대 좌표
const Point* ATetrisGameMode::GetBlockTransform(BlockType type, int32 Rotation = 0)
{
	static Point IBlock[4][4] = 
	{	{ {0,-1},	{1,-1},	{2,-1},		{-1,-1} },
		{ {1,-1},	{1,0},		{1,1},		{1,-2} },
		{ {1,0},	{0,0},		{-1,0},		{2,0} },
		{ {0,0},	{0,-1},	{0,-2},		{0,1} } };
	static Point OBlock[4][4] =
	{	{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} },
		{ {0,-1},	{1,-1},	{1,-2},		{0,-2} } };
	static Point SBlock[4][4] =
	{	{ {0,-1},	{-1,-1},{0,-2},		{1,-2} },
		{ {0,-1},	{0,-2},	{1,-1},		{1,0} },
		{ {0,-1},	{1,-1},	{0,0},		{-1,0} },
		{ {0,-1},	{0,0},	{-1,-1},	{-1,-2} } };
	static Point ZBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {0,-2},		{-1,-2} },
		{ {0,-1},	{0,0},	{1,-1},		{1,-2} },
		{ {0,-1},	{-1,-1},{0,0},		{1,0} },
		{ {0,-1},	{0,-2}, {-1,-1},	{-1,0} } };
	static Point JBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {-1,-1},	{-1,-2} },
		{ {0,-1},	{0,0},	{0,-2},		{1,-2} },
		{ {0,-1},	{-1,-1},{1,-1},		{1,0} },
		{ {0,-1},	{0,-2}, {0,0},		{-1,0} } };
	static Point LBlock[4][4] =
	{	{ {0,-1},	{1,-1}, {-1,-1},	{1,-2} },
		{ {0,-1},	{0,0},	{0,-2},		{1,0} },
		{ {0,-1},	{-1,-1},{1,-1},		{-1,0} },
		{ {0,-1},	{0,-2}, {0,0},		{-1,-2} } };
	static Point TBlock[4][4] =
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

// 해당 좌표에 블록이 배치될 수 있는지 반환
bool ATetrisGameMode::IsLocationPossible(Point location)
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



