// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	SetRootComponent(StaticMeshComp);
	
}

void AWall::SetIsUp(bool isUp)
{
	bIsUp = isUp;
	UStaticMesh* door = bIsUp ? UpDoor : DownDoor;
	StaticMeshComp->SetStaticMesh(door);
	UMaterialInterface* mat = bIsUp ? UpMaterial : DownMaterial;
	if (mat)
	{
		StaticMeshComp->SetMaterial(0, mat);
	}
}
// Called when the game starts or when spawned
void AWall::SetWallData(const FWallData& data)
{
	UpDoor = data.Wall;
	DownDoor = data.WallShort;
	UpMaterial = data.BigWallMat;
	DownMaterial = data.SmallWallMat;
	SetIsUp(true);
}