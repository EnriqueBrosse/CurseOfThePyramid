// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropSpot.generated.h"
class UBoxComponent;

UENUM(BlueprintType)
enum class ESize : uint8
{
	Small = 0 UMETA(DisplayName = "Small"),
	Medium = 1   UMETA(DisplayName = "Medium"),
	Big = 2     UMETA(DisplayName = "Big"),
};

UCLASS()
class CURSEOFTHEPYRAMID_API APropSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropSpot();
private:
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
		ESize PropSize;

};
