#include "Cube.h"


ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 할당
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CUBE"));

	// 스태틱 메쉬를 불러옴
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_CUBE(TEXT("/Game/Tetris/StaticMesh/SM_Cube.SM_Cube"));
	if(SM_CUBE.Succeeded())
	{
		// 불러온 스태틱 메쉬를 컴포넌트의 메쉬로 설정해줌
		Mesh->SetStaticMesh(SM_CUBE.Object);
	}

	// 스태틱 메쉬의 다이나믹 인스턴스를 가져옴
	DynamicInstance = Mesh->CreateDynamicMaterialInstance(0);

}


// 큐브의 색상 및 투명도 설정
void ACube::setColor(BlockType type, float opacity)
{
	FLinearColor Color;

	// 테트리스 블록 형태에 따라 색을 다르게 함
	switch(type)
	{
	case BlockType::I: Color = FLinearColor(0.2f,1.0f,1.0f,opacity);
		break;
	case BlockType::O: Color = FLinearColor(1.0f,1.0f,0.2f,opacity);
		break;
	case BlockType::S: Color = FLinearColor(0.1f,1.0f,0.1f,opacity);
		break;
	case BlockType::Z: Color = FLinearColor(1.0f,0.1f,0.1f,opacity);
		break;
	case BlockType::J: Color = FLinearColor(0.1f,0.1f,1.0f,opacity);
		break;
	case BlockType::L: Color = FLinearColor(1.0f,0.5f,0.2f,opacity);
		break;
	case BlockType::T: Color = FLinearColor(0.5f,0.2f,0.5f,opacity);
		break;
	default : Color = FLinearColor(0.0f,0.0f,0.0f,opacity);
		break;
	}
	DynamicInstance->SetVectorParameterValue(FName(TEXT("Color")),Color);

}


