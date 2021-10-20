// Fill out your copyright notice in the Description page of Project Settings.
#include "Spawner.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"

#include "../OldEnemies/BaseEnemy_01.h" 
#include "../OldEnemies/BaseAIController_01.h"
#include "../Components/HealthComponent.h"
#include "../DataAssets/Dungeon/DA_Spawner.h"

//#include "../LevelGeneration/RoomController.h"
// Sets default values
ASpawner::ASpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actorArray{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager::StaticClass(), actorArray);
	if (actorArray.Num() == 0)
	{
		EnemyManager = GetWorld()->SpawnActor< AEnemyManager>(FVector(0), FRotator(0));
	}
	else
	{
		EnemyManager = Cast<AEnemyManager>(actorArray[0]);
	}
	if (SpawnOnBeginPlay)
	{
		StartSpawning();
	}
}

void ASpawner::StartSpawning()
{
	if (bHasDefeatedRoom /*&& RoomController*/)
	{
		/*RoomController->OpenDoors();*/
		return;
	}
	if (!SpawnData->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnData was not set"));
		bHasDefeatedRoom = true;
		return;
	}
	if (SpawnData->bUseMaxWavesOverride)
	{
		AmountOfWaves = FMath::RandRange(SpawnData->MinAmountOfWaves, SpawnData->MaxAmountOfWaves);
	}
	else
	{
		AmountOfWaves = FMath::RandRange(SpawnData->MinAmountOfWaves, SpawnData->Waves.Num());
	}
	CurrentWave = 0;
	AmountOfEnemies = 0;
	if (SpawnData->Waves.Num() == 0)
	{
		UE_LOG(LogTemp,Warning, TEXT("Spawner doesn't have any waves"))
		return;
	}

	const FWave& wave = SpawnData->Waves[CurrentWave];
	AmountOfEnemies = FMath::RandRange(wave.MinAmountOfEnemies, wave.MaxAmountOfEnemies);
	AmountOfEnemiesAlive = AmountOfEnemies;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpawner::SpawnEnemy, SpawnDelay, false);
}

void ASpawner::SpawnEnemy()
{
	FWave wave;
	if (CurrentWave < SpawnData->Waves.Num())
	{
		wave = SpawnData->Waves[CurrentWave];
	}
	else
	{
		int32 rand = FMath::RandRange(0, SpawnData->Waves.Num() - 1);
		wave = SpawnData->Waves[rand];
	}

	// propability calculation
	// https://softwareengineering.stackexchange.com/questions/150616/get-weighted-random-item
	const TArray<FEnemyData>& enemyData = wave.EnemyData;
	float sum = 0;
	for (size_t i = 0; i < enemyData.Num(); i++)
	{
		sum += enemyData[i].ProbabilityWeight;
	}
	float randNumber = FMath::FRandRange(0, sum);
	FEnemyData enemy{};
	for (size_t i = 0; i < enemyData.Num(); i++)
	{
		randNumber -= enemyData[i].ProbabilityWeight;
		if (randNumber <= 0)
		{
			enemy = enemyData[i];
			break;
		}
	}

	ABaseEnemy_01* spawnedEnemy = EnemyManager->GetEnemy(enemy.ClassToSpawn);
	ABaseAIController_01* controller = Cast < ABaseAIController_01>(spawnedEnemy->Controller);
	controller->OnSpawn(enemy.EnemyData);
	controller->GetHealthComponent()->OnDeath.AddDynamic(this, &ASpawner::EnemyDeath);

	// set position 
	const FTransform& trans = SpawnTransforms[FMath::RandRange(0, SpawnTransforms.Num() - 1)];
	const FTransform& actorTrans = GetActorTransform();
	spawnedEnemy->SetActorTransform(trans * actorTrans);
	AmountOfEnemies--;
	if (AmountOfEnemies != 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpawner::SpawnEnemy, SpawnDelay, false);
	}
}

void ASpawner::EnemyDeath()
{
	AmountOfEnemiesAlive--;
	if (AmountOfEnemiesAlive == 0 /*&& RoomController*/)
	{
		++CurrentWave;
		if (CurrentWave < AmountOfWaves)
		{
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpawner::SpawnEnemy, SpawnDelay, false);
			return;
		}
		bHasDefeatedRoom = true;
		//RoomController->OpenDoors();
	}
}
