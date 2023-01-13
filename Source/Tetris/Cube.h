// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tetris.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class TETRIS_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	ACube();

public:	
	// 큐브의 색과 투명도 설정
	void setColor(BlockType type,float opacity);

	// 메쉬 컴포넌트
	UPROPERTY()
	UStaticMeshComponent *Mesh;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicInstance;


};
