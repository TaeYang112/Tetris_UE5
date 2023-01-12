// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class TETRIS_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void LeftRight(float axis);
	

	void Down(float axis);
	void Drop();
	void ShowMap();
	void RotateRight();
	void RotateLeft();
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle LeftRightTimerHandle;
	UPROPERTY(EditAnywhere,Meta = (AllowPrivateAccess = true))
	FTimerHandle DownTimerHandle;
private:
	UPROPERTY()
	class ATetrisGameMode* TGameMode;

	bool bLeftRight;
	bool bDown;
};
