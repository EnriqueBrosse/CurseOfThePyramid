// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGeneratorStructs.h"
#include "DungeonGenerator.generated.h"
class UGraph;
class UDA_Level;
class ULevelStreamingDynamic;
class ARoomController;

UCLASS()
class CURSEOFTHEPYRAMID_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();
	void TeleportPlayer(int nodeFrom, int nodeTo, AActor* player);

	ARoomController* GetRoomControllerFromID(int nodeId)const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private: 
	UPROPERTY()
		TArray< ARoomController*> RoomControllers;
	UPROPERTY(EditAnywhere)
		float GridSize = 7000.f;
	UFUNCTION()
		void OnLevelLoaded();
	UFUNCTION()
		void AddRoomController();

	void LoadAndUnloadLevels(int nodeTo);
	void MakeDungeonGrid();

	UPROPERTY(EditAnywhere)
		UDA_Level* Level;
	UPROPERTY(EditAnywhere)
		TArray<FString> FlexibleLevels{};
	FVector LocationOffset{ 0 };
	int AmountOfRows;
	int AmountOfColumns;
	UPROPERTY()
		UGraph* Graph;
	UPROPERTY(EditAnywhere)
		TArray<ULevelStreamingDynamic*> StreamedLevels;
	FVector GetNodePosition(int nodeId);

};
