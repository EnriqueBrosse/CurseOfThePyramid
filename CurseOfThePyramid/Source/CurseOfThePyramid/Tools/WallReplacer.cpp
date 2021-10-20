// Fill out your copyright notice in the Description page of Project Settings.


#include "WallReplacer.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "../Dungeon/Room/Wall.h"
//#include "../DataAssets/DA_WallDataBase.h"
// Sets default values
AWallReplacer::AWallReplacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWallReplacer::ReplaceWalls()
{
	TArray<AActor*> actorArray{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), actorArray);
	TArray<AStaticMeshActor*> wallArray{};
	wallArray.Reserve(actorArray.Num());
	for (int32 i = 0; i < actorArray.Num(); i++)
	{
		wallArray.Add(Cast<AStaticMeshActor>(actorArray[i]));
	}
	actorArray.Empty();
	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TArray<AStaticMeshActor*> deleteArray{};
	for (size_t i = 0; i < wallArray.Num(); i++)
	{
		bool succeed = false;
		FWallData wallData = GetApproperiateData(wallArray[i]->GetStaticMeshComponent()->GetStaticMesh(), succeed);
		if (succeed)
		{
			AWall* wall = GetWorld()->SpawnActor<AWall>(WallBP, wallArray[i]->GetTransform(), spawnParams);
			wall->SetWallData(wallData);
			deleteArray.Add(wallArray[i]);
		}
	}
	for (size_t i = 0; i < deleteArray.Num(); i++)
	{
		deleteArray[i]->Destroy();
	}
}

FWallData AWallReplacer::GetApproperiateData(UStaticMesh* staticMesh, bool& succeeded)
{
	succeeded = false;
	//if (!DataBase)
	//{
	//	return FWallData{};
	//}
	//const TArray<FWallData>& wallData = DataBase->WallData;
	//for (size_t i = 0; i < wallData.Num(); i++)
	//{
	//	if (wallData[i].Wall == staticMesh || wallData[i].WallShort == staticMesh)
	//	{
	//		succeeded = true;
	//		return wallData[i];
	//	}
	//}
	return FWallData{};
}