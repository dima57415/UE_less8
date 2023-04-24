// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "DamageTaker.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;	

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}



void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner(); // ACannon
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	//UE_LOG(LogTemp, Warning, TEXT("Overload actro: %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	if (OtherActor != owner || OtherActor != OwnerByOwner)
	{
		IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);
		if (DamageActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			DamageActor->TakeDamage(damageData);
		}
		else
		{
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					//mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
					mesh->AddForce(forceVector * PushForce, NAME_None, true);
				}
			} 
			else
			{
				OtherActor->Destroy();
			}
		}
		if(onExplose)
			Explode();

		Destroy();
	}
}


void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(500);
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
		DamageOrImpulse(AttackHit);
	}
}


void AProjectile::DamageOrImpulse(TArray<FHitResult> AttackHit)
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
