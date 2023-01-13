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
	// ť���� ���� ���� ����
	void setColor(BlockType type,float opacity);

	// �޽� ������Ʈ
	UPROPERTY()
	UStaticMeshComponent *Mesh;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicInstance;


};
