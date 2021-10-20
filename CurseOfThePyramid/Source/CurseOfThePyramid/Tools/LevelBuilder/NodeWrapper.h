// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Dungeon/Generation/LevelGeneratorStructs.h"
#include "NodeWrapper.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class CURSEOFTHEPYRAMID_API ANodeWrapper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeWrapper();
	virtual void OnConstruction(const FTransform& Transform) override;

	FORCEINLINE ERoomType GetRoomType()const { return RoomType; }
	FORCEINLINE const FString& GetLevelPath()const { return LevelPath; }
	UPROPERTY(EditAnywhere)
		int NodeID;
private:
	UPROPERTY(EditAnywhere)
		UMaterialInstance* FixedLevelMat;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* InterchangableLevelMat;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* KeyLevelMat;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* RoomBeforeBossLevelMat;	
	UPROPERTY(EditAnywhere)
		UMaterialInstance* StartLevelMat;

	UPROPERTY(EditAnywhere)
		ERoomType RoomType = ERoomType::Interchangable;
	UPROPERTY(EditAnywhere)
		FString LevelPath;
	UPROPERTY(EditAnywhere)
		UStaticMesh* StaticMesh;
	UStaticMeshComponent* StaticMeshComp;
};
