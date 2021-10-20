// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"
class ABaseEnemy_01;
class AMainPlayerController;

UCLASS()
class CURSEOFTHEPYRAMID_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

	ABaseEnemy_01* GetEnemy(TSubclassOf<ABaseEnemy_01> enemyClass);
protected: 
	virtual void BeginPlay() override;

private:
	TMap<TSubclassOf<ABaseEnemy_01>, TArray<ABaseEnemy_01*>> _enemyMap;
	AMainPlayerController* PlayerController;
	UFUNCTION()
	void SwitchWorld(bool isSoul);
};
