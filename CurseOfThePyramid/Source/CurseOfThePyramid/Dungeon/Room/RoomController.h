// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomController.generated.h"
class ADoor;
class AWall;
class APropSpot;
class ASpawner;
class UGraph;
class ADungeonGenerator;

USTRUCT(BlueprintType)
struct FWallArr
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray< AWall*> WallArray;
};

UCLASS()
class CURSEOFTHEPYRAMID_API ARoomController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomController();
	void SetLevelData(int nodeId, UGraph* graph, int rotation, ADungeonGenerator* dunGen);
	void TeleportPlayer(int toId, AActor* player);
	void OpenDoors();
	// do not call this during runtime
	void SetupEnvironment();

	FTransform GetTeleportTransformFromDoor(int id)const;
	FORCEINLINE int GetId()const { return ID; }
	FORCEINLINE ASpawner* GetSpawner()const { return Spawner; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	UPROPERTY(EditAnywhere)
		int ID = -1;

	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* NDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* NEDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* EDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* SEDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* SDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* SWDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* WDoor;
	UPROPERTY(EditAnywhere, Category = "Door")
		ADoor* NWDoor;

	UPROPERTY()
		ADungeonGenerator* DungeonGenerator;
	UPROPERTY()
		TArray<ADoor*> Doors;
	UPROPERTY(EditAnywhere, Category = "PropSpots")
		TArray<APropSpot*> PropsToReplace;
	UPROPERTY(EditAnywhere, Category = "PropSpots")
		TArray<TSubclassOf<APropSpot>> RandomProps;

	void SetupDoors(UGraph* graph, int rotation);
	void SetupWalls(int rotation);

	// call in editor functions

	UFUNCTION(CallInEditor, Category = "PropSpots")
		void FindPropSpots();
	UPROPERTY(EditAnywhere)
		TArray<FWallArr> WallArray;
	UFUNCTION(CallInEditor)
		void FindWalls();
	UPROPERTY(EditAnywhere)
		ASpawner* Spawner;
	UFUNCTION(CallInEditor)
		void FindDoors();

	void FindSpawner();
};
