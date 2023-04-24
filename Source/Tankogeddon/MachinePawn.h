// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "MachinePawn.generated.h"


class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API AMachinePawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	AMachinePawn();

	void Fire();
	void FireSpecial();

	void ChangeGun();
	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	ACannon* GetCannon();

	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void DamageTaked(float Value);

	UFUNCTION()
	void Die();


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHeathlComponent* HealthComponent;


	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;

};
