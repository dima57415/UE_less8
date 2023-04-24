// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "Engine/TargetPoint.h"
//#include "HeathlComponent.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()

public:
	ATankFactory();

	virtual void  TakeDamage(FDamageData DamageData) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void DamageTaked(float DamageValue);

	UFUNCTION()
		void Die();

	void SpawnNewTank();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
		TSubclassOf<class ATankPawn> TankSpawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn params")
		float SpawnTankRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		TArray<ATargetPoint*> TankWayPoints;

};
