// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Cannon.h"
#include <Kismet/KismetMathLibrary.h>
#include "HeathlComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;



	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
		BodyMesh->SetStaticMesh(BodyMeshTemp);

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
		TurretMesh->SetStaticMesh(TurretMeshTemp);


	CannonSetupPointTemp = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPointTest"));
	CannonSetupPointTemp->SetupAttachment(TurretMesh);
  
}


void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimer;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimer, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Destroyed()
{
	if (Cannon)
		Cannon->Destroy();
}


void ATurret::Targeting()
{
	if (!PlayerPawn)
		return;

	if (!IsPlayerSeen())
		return;

	if (IsPlayerInRange())
	{
		RotateToPlayer();

		if (CanFire())
		{
			Fire();
		}
	}
}

void ATurret::RotateToPlayer() 
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire() 
{
	FVector targetDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetDir, DirToPlayer)));
	return aimAngle <= Accurency;

}

FVector ATurret::GetEyesPosition() const
{
	return CannonSetupPointTemp->GetComponentLocation();
}






bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = GetEyesPosition();
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);

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



