// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../Dungeon/Generation/LevelGeneratorStructs.h"
#include "DA_Level.generated.h"

UCLASS()
class CURSEOFTHEPYRAMID_API UDA_Level : public UDataAsset
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere)
		int AmountOfColumns = 5;
	UPROPERTY(EditAnywhere)
		int AmountOfRows = 5;
	UPROPERTY(EditAnywhere)
		TArray<FGraphNode> Nodes;
	UPROPERTY(EditAnywhere)
		TArray<FConnection> Connections;
};
