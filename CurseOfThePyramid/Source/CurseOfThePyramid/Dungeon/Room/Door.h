// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ARoomController;

UCLASS()
class CURSEOFTHEPYRAMID_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void MoveDoor(bool open);

	FORCEINLINE void SetActive(bool active) { bIsActive = active; }
	FORCEINLINE bool GetIsActive()const { return bIsActive; }
	FORCEINLINE void SetToConnectionId(int id) { ToConnectionId = id; }
	FORCEINLINE	int GetToConnectionId()const { return ToConnectionId; }
	FORCEINLINE void SetRoomController(ARoomController* roomController) { RoomController = roomController; }
	FORCEINLINE const FTransform& GetTeleportTransform()const { return TeleportLocationFromOtherDoor; }
protected: 
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* TeleportOverlap;
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		FTransform TeleportLocationFromOtherDoor {};

	UPROPERTY(EditAnywhere, Category = "Moving", Meta = (MakeEditWidget = true))
		FVector ClosedLocation {};
	UPROPERTY(EditAnywhere, Category = "Moving", Meta = (MakeEditWidget = true))
		FVector OpenedLocation {};
	UPROPERTY()
		ARoomController* RoomController;
	bool bIsOpen = false;
	bool bIsActive = false;
	UPROPERTY(EditAnywhere)
		int ToConnectionId = -1;
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void SetTickOff();
};