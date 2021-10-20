// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Generation/LevelGeneratorStructs.h"
#include "Orientation.h"
#include "Wall.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

UCLASS()
class CURSEOFTHEPYRAMID_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();
	void SetIsUp(bool isUp);
	FORCEINLINE EWindOrientation GetOrientation()const { return DoorOrientation; }
	void SetWallData(const FWallData& data);

private: 
	UPROPERTY(EditAnywhere)
		bool bIsUp = true;
	UPROPERTY(EditAnywhere)
		EWindOrientation DoorOrientation = EWindOrientation::UnKnown;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
		UStaticMesh* UpDoor;
	UPROPERTY(EditAnywhere)
		UStaticMesh* DownDoor;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* UpMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* DownMaterial;
};
