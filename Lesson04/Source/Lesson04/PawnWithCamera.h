// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RunTime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "RunTime/Engine/Classes/Camera/CameraComponent.h"
#include "PawnWithCamera.generated.h"

UCLASS()
class LESSON04_API APawnWithCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "PWCamera")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "PWCamera")
		UCameraComponent* OurCamera;

	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void CameraPitch(float AxisValue);
	void CameraYaw(float AxisValue);

	void ZoomIn();
	void ZoomOut();
	
};
