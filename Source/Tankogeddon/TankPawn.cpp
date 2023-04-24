// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include <Kismet/KismetMathLibrary.h>
#include "Cannon.h"
#include <Components/ArrowComponent.h>
#include "HeathlComponent.h"
#include "Engine/TargetPoint.h"

// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;


	CannonSetupPointTemp = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPointTest"));
	CannonSetupPointTemp->SetupAttachment(TurretMesh);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}


// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ATankPawn::MoveAll(DeltaTime);

	float yawRotation = RotationSpeed * targetRotationRight * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation += currentRotation.Yaw;
	FRotator newRotator = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotator);


	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		//FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		//FRotator TurretRotation = TurretMesh->GetComponentRotation();
		//targetRotation.Pitch = TurretRotation.Pitch;
		//targetRotation.Roll = TurretRotation.Roll;
		//TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey));
		RotateTurretTo(mousePos);

	}
}

TArray<FVector> ATankPawn::GetPatrollingPath() const
{
	TArray<FVector> points;
	for (ATargetPoint* point : PatrollingPath)
	{
		points.Add(point->GetActorLocation());
	}
	return points;
}


void ATankPawn::SetPattrollingPath(TArray<ATargetPoint*> newPatrollingPath)
{
	PatrollingPath = newPatrollingPath;
}


void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	targetRightAxisValue = Value;
}

void ATankPawn::MoveAll(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector ForwardMove = ForwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime;
	FVector RightMove = RightVector * MoveSpeed * targetRightAxisValue * DeltaTime;
	FVector Move = CurrentPosition + ForwardMove + RightMove;
	SetActorLocation(Move, true);
}

void ATankPawn::RotateRight(float Value)
{
	targetRotationRight = Value;
}


void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
}

FVector ATankPawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FVector mousePos = TankController->GetMousePosition();
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	
	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey));
}


FVector ATankPawn::GetEyesPosition() const
{
	return CannonSetupPointTemp->GetComponentLocation();
}


