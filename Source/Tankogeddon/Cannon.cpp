// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "DamageTaker.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"



ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CannonMesh->SetCollisionProfileName("NoCollision");

	ProjectileSpawnComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnComponent->SetupAttachment(CannonMesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnComponent);
	ShootEffect->SetAutoActivate(false);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	AudioEffect->SetupAttachment(SceneComp);
	AudioEffect->SetAutoActivate(false);

	//ProjectileSpawnComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
	bDelayToFire = true;
	Ammo = 10;
}

void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::Fire()
{
	if (!bReadyToFire || !bDelayToFire || Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Can't shooting");
		return;
	}

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	if (CannonType == ECannonType::FireProjectile)
	{
		Ammo -= 1;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Fire Projectile, Ammo %d"), Ammo));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnComponent->GetComponentLocation(), ProjectileSpawnComponent->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else
	{
		Ammo -= 1;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Fire trace, Ammo %d"), Ammo));

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;


		FVector Start = ProjectileSpawnComponent->GetComponentLocation();
		FVector End = Start + ProjectileSpawnComponent->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 1.0f, 0, 5);
			if (hitResult.GetActor())
			{
				//AActor* OverlappedActor = hitResult.GetActor();
				//UE_LOG(LogTemp, Warning, TEXT("Actor %s. "), *OverlappedActor->GetName());
				//OverlappedActor->Destroy();
				IDamageTaker* DamageActor = Cast<IDamageTaker>(hitResult.GetActor());
				if (DamageActor)
				{
					FDamageData damageData;
					damageData.DamageMaker = this;
					damageData.DamageValue = 1.0f;
					damageData.Instigator = GetOwner();
					

					DamageActor->TakeDamage(damageData);
				}
				else
				{
					hitResult.GetActor()->Destroy();
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}

	}

	if (Ammo == 0)
	{
		bReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
	
	bDelayToFire = false;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ACannon::DelayFire_, FireDelay, false);
}

void ACannon::FireSpecial()
{
	if (!bReadyToFire || !bDelayToFire || Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Can't shooting");
		return;
	}
	if (CannonSpecialType == ECannonSpecialType::SpecialFire)
	{
		Ammo -= 1;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Fire Special, Ammo %d"), Ammo));
	}

	if (Ammo == 0)
	{
		bReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
	bDelayToFire = false;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ACannon::DelayFire_, FireDelay, false);
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
	Ammo = 10;
}

void ACannon::DelayFire_()
{
	bDelayToFire = true;
}

void ACannon::AddAmmo(uint32_t _Ammo)
{
	Ammo = Ammo + _Ammo;
}



