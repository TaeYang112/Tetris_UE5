#include "Cube.h"


ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = false;

	// ������Ʈ �Ҵ�
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CUBE"));

	// ����ƽ �޽��� �ҷ���
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_CUBE(TEXT("/Game/Tetris/StaticMesh/SM_Cube.SM_Cube"));
	if(SM_CUBE.Succeeded())
	{
		// �ҷ��� ����ƽ �޽��� ������Ʈ�� �޽��� ��������
		Mesh->SetStaticMesh(SM_CUBE.Object);
	}

	// ����ƽ �޽��� ���̳��� �ν��Ͻ��� ������
	DynamicInstance = Mesh->CreateDynamicMaterialInstance(0);

}


// ť���� ���� �� ���� ����
void ACube::setColor(BlockType type, float opacity)
{
	FLinearColor Color;

	// ��Ʈ���� ��� ���¿� ���� ���� �ٸ��� ��
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


