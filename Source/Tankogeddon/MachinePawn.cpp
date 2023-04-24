// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Cannon.h"
#include <Components/ArrowComponent.h>
#include "HeathlComponent.h"

// Sets default values
AMachinePawn::AMachinePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);


	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHeathlComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AMachinePawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMachinePawn::DamageTaked);

}
void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(CannonClass);
}

void AMachinePawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AMachinePawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void AMachinePawn::ChangeGun()
{
	TSubclassOf<ACannon> TempCannon = CannonClass;
	CannonClass = SecondCannonClass;
	SecondCannonClass = TempCannon;
	SetupCannon(CannonClass);
}


void AMachinePawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}

	CannonClass = newCannonClass;

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


ACannon* AMachinePawn::GetCannon()
{
	return Cannon;
}


void AMachinePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AMachinePawn::DamageTaked(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Value);
}

void AMachinePawn::Die()
{
	if (Cannon)
		Cannon->Destroy();
	Destroy();
}