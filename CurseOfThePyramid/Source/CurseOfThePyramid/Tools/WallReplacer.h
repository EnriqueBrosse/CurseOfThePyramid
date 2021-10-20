// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Dungeon/Generation/LevelGeneratorStructs.h"
#include "WallReplacer.generated.h"
//class UDA_WallDataBase;
class UStaticMesh;
class AWall;
UCLASS()
class CURSEOFTHEPYRAMID_API AWallReplacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallReplacer();
	UFUNCTION(CallInEditor)
		void ReplaceWalls(); 
	//UPROPERTY(EditAnywhere)
	//	UDA_WallDataBase* DataBase;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWall> WallBP;
	FWallData GetApproperiateData(UStaticMesh* staticMesh, bool& succeeded);
};
