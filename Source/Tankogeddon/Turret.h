// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachinePawn.h"
#include "TankPawn.h"
#include "Turret.generated.h"


class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATurret : public AMachinePawn
{
	GENERATED_BODY()
	
public:	
	ATurret();

	
protected:

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPointTemp;

	const FString BodyMeshPath =
		"StaticMesh'/Game/CSC/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath =
		"StaticMesh'/Game/CSC/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";



	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();

	FVector GetEyesPosition() const;
	bool IsPlayerSeen();

};
