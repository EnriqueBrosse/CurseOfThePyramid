// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Dungeon/Room/Orientation.h"
#include "ConnectionWrapper.generated.h"
class UStaticMeshComponent;


UCLASS()
class CURSEOFTHEPYRAMID_API AConnectionWrapper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConnectionWrapper();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere)
		int FromNodeID;
	UPROPERTY(EditAnywhere)
		int ToNodeID;

	UPROPERTY(EditAnywhere)
		EWindOrientation Dir = EWindOrientation::East;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComp;
};
