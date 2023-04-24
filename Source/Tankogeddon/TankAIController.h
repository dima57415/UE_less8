// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATankPawn* TankPawn;

	UPROPERTY()
	class APawn* PlayerPawn;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccurency = 0.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float DelayChange = 5.0f;


	int32 CurrentPatrollingIndex = 0;

	float GetRotationgValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();

	bool IsPlayerSeen();
	void ChangeGun();
	void Initialize();

	FTimerHandle ChangeTimer;
};
