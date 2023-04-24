// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"

#include "DamageTaker.h"
#include "PhysicsComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailE ffect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	moveVector = GetActorForwardVector() * TrajectorySpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), moveVector, TrajectoryMaxTime, TrajectoryTimeStep, 0.0f);
	if (bShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5.0f, 8.0f, FColor::Purple, true, 1.0f, 0, 2);
		}
	}
	TragectoryCurrentIndex = 0;
	Super::Start();
	//Move();
}
void APhysicsProjectile::Move()
{
	FVector currentMoveVector = CurrentTrajectory[TragectoryCurrentIndex] - GetActorLocation();
	currentMoveVector.Normalize();
	FVector newLocation = GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);
	if (FVector::Distance(newLocation, CurrentTrajectory[TragectoryCurrentIndex]) <= MoveAccurency)
	{
		TragectoryCurrentIndex++;
		if (TragectoryCurrentIndex >= CurrentTrajectory.Num())
		{
			//Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryCurrentIndex]);
			SetActorRotation(newRotation);
		}
	}
}


void APhysicsProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "ExplodeTrace";

	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		startPos,
		endPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	if (sweepResult)
	{
		for (FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if (!otherActor)
				continue;
			IDamageTaker* damageTakerActor = Cast<IDamageTaker>(otherActor);
			if (damageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
			else
			{
				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
				if (mesh)
				{
					if (mesh->IsSimulatingPhysics())
					{
						FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
					}
				}
			}
		}
	}
}


