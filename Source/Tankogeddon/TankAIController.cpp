// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"





void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPawn)
		Initialize();
	if (!TankPawn)
		return;

	TankPawn->MoveForward(1.0f);

	TankPawn->RotateRight(GetRotationgValue());

	Targeting();
}


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();

	GetWorld()->GetTimerManager().SetTimer(ChangeTimer, this, &ATankAIController::ChangeGun, DelayChange, true);
}


void ATankAIController::ChangeGun()
{
	TankPawn->ChangeGun();
}


float ATankAIController::GetRotationgValue()
{
	FVector currentPoint = PatrollingPoints[CurrentPatrollingIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();

	if (FVector::Distance(currentPoint, PawnLocation) <= MovementAccurency)
	{
		CurrentPatrollingIndex++;
		if (CurrentPatrollingIndex >= PatrollingPoints.Num())
			CurrentPatrollingIndex = 0;
	}

	FVector moveDirection = currentPoint - PawnLocation;
	moveDirection.Normalize();

	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	//DrawDebugLine(GetWorld(), PawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));
	float rotationValue = 0.0f;
	if (forwardAngle > 2.0f)
		rotationValue = 1.0f;
	if (rightAngle > 90)
		rotationValue = -rotationValue;

	return rotationValue;
}

void ATankAIController::Targeting() 
{
	if (!IsPlayerSeen())
		return;

	if (CanFire())
		Fire();
	else
		RotateToPlayer();
}

void ATankAIController::RotateToPlayer()
{
	if (IsPlayerInRange())
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}

bool ATankAIController::IsPlayerInRange()
{
	if (!PlayerPawn)
		return false;

	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;

}

bool ATankAIController::CanFire()
{
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}



bool ATankAIController::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetEyesPosition();
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(TankPawn);

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, params))
	{
		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Purple, false, 0.5f, 0, 10);
			return hitResult.GetActor() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Red, false, 0.5f, 0, 10);
	return false;
}


void ATankAIController::Initialize()
{
	TankPawn = Cast<ATankPawn>(GetPawn());

	if (!TankPawn)
		return;
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPath();
	for (FVector point : points)
	{
		PatrollingPoints.Add(point);
	}
	CurrentPatrollingIndex = 0;
}


