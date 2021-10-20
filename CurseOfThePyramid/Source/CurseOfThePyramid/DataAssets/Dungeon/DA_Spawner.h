// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Spawner.generated.h"
class ABaseEnemy_01;
class UDA_EnemyData;

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseEnemy_01> ClassToSpawn;
	UPROPERTY(EditAnywhere)
		UDA_EnemyData* EnemyData;

	// the probability of the enemy spawning
	UPROPERTY(EditAnywhere)
		float ProbabilityWeight = 1.f;
};

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int MinAmountOfEnemies;
	UPROPERTY(EditAnywhere)
		int MaxAmountOfEnemies;
	UPROPERTY(EditAnywhere)
		TArray<FEnemyData> EnemyData;
}; 

UCLASS()
class CURSEOFTHEPYRAMID_API UDA_Spawner : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FWave> Waves;
	UPROPERTY(EditAnywhere)
		int MinAmountOfWaves;
	UPROPERTY(EditAnywhere)
		int MaxAmountOfWaves;
	UPROPERTY(EditAnywhere)
		bool bUseMaxWavesOverride = false;

};
