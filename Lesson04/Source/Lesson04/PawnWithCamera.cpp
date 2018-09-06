// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnWithCamera.h"


// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnWithCamera::Tick(float DeltaTime)
{
	{
		Super::Tick(DeltaTime);
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime * 0.5f;
		}
		else
		{
			ZoomFactor -= DeltaTime * 0.5f;
		}
		ZoomFactor = FMath::Clamp(ZoomFactor, 0.0f, 1.0f);
		OurCamera->FieldOfView = FMath::Lerp(90.0f, 60.0f, ZoomFactor);
		SpringArm->TargetArmLength = FMath::Lerp(400.0f, 300.0f, ZoomFactor);
	}

	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp((NewRotation.Pitch + CameraInput.Y), -80.0f, -15.0f);
		SpringArm->SetWorldRotation(NewRotation);
	}

	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector()*MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
	
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	PlayerInputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::CameraPitch);
	PlayerInputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::CameraYaw);
}

void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::CameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::CameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}

