// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	virtual void Start();

	void Explode();
	void DamageOrImpulse(TArray<FHitResult> AttackHit);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Damage = 1.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	float PushForce = 1000.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Exploseve")
	bool onExplose = 1.0f;


	FTimerHandle MovementTimer;

	UFUNCTION()
	virtual void Move();

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
			OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult);

public:	

};
