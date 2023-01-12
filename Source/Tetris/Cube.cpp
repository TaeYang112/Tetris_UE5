// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"


// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CUBE"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_CUBE(TEXT("/Game/Tetris/StaticMesh/SM_Cube.SM_Cube"));
	if(SM_CUBE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_CUBE.Object);
	}

	DynamicInstance = Mesh->CreateDynamicMaterialInstance(0);

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube::setColor(BlockType type, float opacity)
{
	FLinearColor Color;
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


