// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemyManager;
class ARoomController;
class UDA_Spawner;

UCLASS()
class CURSEOFTHEPYRAMID_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawner();
	FORCEINLINE bool IsDoneSpawning()const { return bIsDoneSpawning; }
	void StartSpawning();
	void SetRoomController(ARoomController* roomController) { RoomController = roomController; }
	void SetSpawnData(UDA_Spawner* spawnData) { SpawnData = spawnData; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UDA_Spawner* SpawnData;
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		TArray<FTransform> SpawnTransforms;
	UPROPERTY(EditAnywhere)
		bool SpawnOnBeginPlay = false;


	UFUNCTION()
		void EnemyDeath();
	bool bHasDefeatedRoom = false;
	int CurrentWave = 0;
	int AmountOfWaves = 0;
	int AmountOfEnemies = 0;
	int AmountOfEnemiesAlive = 0;
	UPROPERTY(EditAnywhere)
		float SpawnDelay = 0.1f;

	bool bIsDoneSpawning = false;
	AEnemyManager* EnemyManager = nullptr;
	ARoomController* RoomController = nullptr;
	void SpawnEnemy();
	FTimerHandle TimerHandle;
};
