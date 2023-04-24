// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
	GENERATED_BODY()

public:
	AMapLoader();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MapLoaderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName LevelName = "";
};
