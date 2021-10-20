// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Dungeon/Room/Orientation.h"
#include "LevelBuilder.generated.h"

class UDA_Level;
class UStaticMeshComponent;

UCLASS()
class CURSEOFTHEPYRAMID_API ALevelBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelBuilder();
	virtual void OnConstruction(const FTransform& Transform) override;
private:
	UPROPERTY(EditAnywhere)
		UDA_Level* LevelToReplace;
	UFUNCTION(CallInEditor)
		void MakeLevel();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
		int Rows = 10; 
	UPROPERTY(EditAnywhere)
		int Colums = 10;

	int GetNodeID(const FVector& pos, const FVector& center);

	int GetOtherNodeId(int fromId, EWindOrientation dir);
};
