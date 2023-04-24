// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachinePawn.h"
#include "DamageTaker.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class ACannon;
class ATargetPoint;

UCLASS()
class TANKOGEDDON_API ATankPawn : public AMachinePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveAll(float DeltaTime);
	void RotateRight(float Value);

	UFUNCTION()
	TArray<FVector> GetPatrollingPath() const;
	void SetPattrollingPath(TArray<ATargetPoint*> newPatrollingPath);


	UFUNCTION()
	float GetMovementAccurency() { return MovementAccurency; };

	FVector GetTurretForwardVector() const;
	void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (MakeEditWidget = true))
	TArray<ATargetPoint*> PatrollingPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MovementAccurency = 50;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Rotation")
	float TurretRotationInterpolationKey = 0.1f;


	UPROPERTY()
	class ATankPlayerController* TankController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPointTemp;


	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f;
	float targetRotationRight = 0.0f;
};
