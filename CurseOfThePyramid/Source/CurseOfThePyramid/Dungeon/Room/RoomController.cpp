// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomController.h"
#include "PropSpot.h"
#include "Door.h"
#include "Wall.h"
#include "Kismet/GameplayStatics.h"
#include "../Generation/Graph.h"
#include "../Generation/DungeonGenerator.h"
#include "../../Managers/Spawner.h"

// Sets default values
ARoomController::ARoomController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARoomController::BeginPlay()
{
	Super::BeginPlay();

	if (PropsToReplace.Num() > 0 && RandomProps.Num() > 0)
	{
		const int maxRand = RandomProps.Num() - 1;
		for (size_t i = 0; i < PropsToReplace.Num(); i++)
		{
			const FTransform& trans = PropsToReplace[i]->GetActorTransform();
			const int rand = FMath::RandRange(0, maxRand);
			PropsToReplace[i]->Destroy();
			GetWorld()->SpawnActor<APropSpot>(RandomProps[rand], trans.GetLocation(), trans.GetRotation().Rotator());
		}
	}
	if (Spawner)
	{
		Spawner->SetRoomController(this);
	}
}

void ARoomController::SetLevelData(int nodeId, UGraph* graph, int rotation, ADungeonGenerator* dunGen)
{
	DungeonGenerator = dunGen;
	ID = nodeId;
	SetupDoors(graph,rotation);
	SetupWalls(rotation);
	if (!Spawner)
	{
		OpenDoors();
	}
}

void ARoomController::SetupDoors(UGraph* graph, int rotation)
{
	Doors.Empty();
	// Padding of the array 
	// [0] N  [1] NE [2] E, [3] SE, 
	// [4] S  [5] SW [6] W  [7] NW
	Doors.Add(NDoor);
	Doors.Add(NEDoor);
	Doors.Add(EDoor);
	Doors.Add(SEDoor);
	Doors.Add(SDoor);
	Doors.Add(SWDoor);
	Doors.Add(WDoor);
	Doors.Add(NWDoor);
	TArray<ADoor*> tempDoors{ Doors };

	// calculate the offset 
	int offset = 0;
	int offsetMinMax = offset - tempDoors.Num();
	switch (rotation)
	{
	case 0:
		offsetMinMax = 0;
		break;
	case 90:
		offset = 2;
		offsetMinMax = offset - tempDoors.Num();
		break;
	case 180:
		offset = 4;
		offsetMinMax = offset - tempDoors.Num();
		break;
	case 270:
		offset = 6;
		offsetMinMax = offset - tempDoors.Num();
		break;
	default:
		break;
	}

	int index = 0;
	for (size_t i = offsetMinMax; i < tempDoors.Num(); i++)
	{
		Doors[i] = tempDoors[index];
		index++;
	}
	for (size_t i = 0; i < offsetMinMax; i++)
	{
		Doors[i] = tempDoors[i + offset];
	}
	tempDoors.Empty();

	// checking where the door is connected to
	const int rowSize = graph->GetRowSize();
	const TArray<FConnection>& connections = graph->GetFromConnections(ID);
	for (size_t i = 0; i < connections.Num(); i++)
	{
		const int to = connections[i].To;
		EWindOrientation orientation = EWindOrientation::UnKnown;
		if (to == (ID + rowSize))// checking the north
		{
			orientation = EWindOrientation::North;
		}
		else if (to == (ID - rowSize))// checking the south
		{
			orientation = EWindOrientation::South;
		}
		else if (to == (ID + 1)) // east
		{
			orientation = EWindOrientation::East;
		}
		else if (to == (ID - 1)) // west
		{
			orientation = EWindOrientation::West;
		}
		else if (to == (ID + rowSize + 1))
		{
			orientation = EWindOrientation::NorthEast;
		}
		else if (to == (ID - rowSize + 1))
		{
			orientation = EWindOrientation::SouthEast;
		}
		else if (to == (ID - rowSize - 1))
		{
			orientation = EWindOrientation::SouthWest;
		}
		else if (to == (ID + rowSize - 1))
		{
			orientation = EWindOrientation::NorthWest;
		}

		if (orientation == EWindOrientation::UnKnown)
		{
			UE_LOG(LogTemp, Error, L"Door orientation is unknown");
			continue;
		}

		if (Doors[int(orientation)])
		{
			Doors[int(orientation)]->SetToConnectionId(to);
			Doors[int(orientation)]->SetActive(true);
			Doors[int(orientation)]->MoveDoor(false);
			Doors[int(orientation)]->SetRoomController(this);
		}
	}
}

void ARoomController::SetupWalls(int rotation)
{
	EWindOrientation wall1ToDissapear{};
	EWindOrientation wall2ToDissapear{};
	TArray<FWallArr> tempWallArr = WallArray;
	int offset = 0;
	int offsetMinMax = offset - tempWallArr.Num();
	bool inverse = false;
	switch (rotation)
	{
	case 0:
		offsetMinMax = 0;
		offsetMinMax = offset - tempWallArr.Num();
		wall1ToDissapear = EWindOrientation::East;
		wall1ToDissapear = EWindOrientation::South;
		break;
	case 90:
		offset = 2;
		offsetMinMax = offset - tempWallArr.Num();
		wall1ToDissapear = EWindOrientation::North;
		wall1ToDissapear = EWindOrientation::East;
		break;
	case 180:
		offset = 4;
		offsetMinMax = offset - tempWallArr.Num();
		wall1ToDissapear = EWindOrientation::NorthEast;
		wall1ToDissapear = EWindOrientation::West;
		inverse = true;
		break;
	case 270:
		offset = 6;
		offsetMinMax = offset - tempWallArr.Num();
		break;
	default:
		break;
	}
	int index = 0;
	for (size_t i = offsetMinMax; i < tempWallArr.Num(); i++)
	{
		WallArray[i] = tempWallArr[index];
		index++;
	}
	for (size_t i = 0; i < offsetMinMax; i++)
	{
		WallArray[i] = tempWallArr[i + offset];
	}
	tempWallArr.Empty();
	for (int i = 0; i < WallArray.Num(); i++)
	{
		bool isUp = true;
		if ((int(wall1ToDissapear) >= i && int(wall2ToDissapear) <= i) && !inverse
			|| (int(wall1ToDissapear) <= i && int(wall2ToDissapear) >= i) && inverse)
		{
			isUp = false;
		}
		TArray< AWall*> wallArr = WallArray[i].WallArray;
		for (size_t j = 0; j < wallArr.Num(); j++)
		{
			wallArr[j]->SetIsUp(isUp);
		}
	}
}


void ARoomController::TeleportPlayer(int toId, AActor* player)
{
	DungeonGenerator->TeleportPlayer(ID, toId, player);
}
void ARoomController::OpenDoors()
{
	for (size_t i = 0; i < Doors.Num(); i++)
	{
		if (Doors[i] && Doors[i]->GetToConnectionId() != INDEX_NONE)
		{
			Doors[i]->MoveDoor(true);
		}
	}
}
// do not call this during runtime
void ARoomController::SetupEnvironment()
{
	FindPropSpots();
	FindWalls();
	FindSpawner();
	FindDoors();
}

FTransform ARoomController::GetTeleportTransformFromDoor(int id)const
{
	for (size_t i = 0; i < Doors.Num(); i++)
	{
		if (Doors[i] && Doors[i]->GetToConnectionId() == id)
		{
			return Doors[i]->GetTeleportTransform() * Doors[i]->GetActorTransform();
		}
	}
	UE_LOG(LogTemp, Warning, L"couldn't find the right ID from door");
	return FTransform();
}

void ARoomController::FindPropSpots()
{
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APropSpot::StaticClass(), FoundActors);
	PropsToReplace.Empty();
	for (size_t i = 0; i < FoundActors.Num(); i++)
	{
		PropsToReplace.Add(Cast<APropSpot>(FoundActors[i]));
	}
}

void ARoomController::FindWalls()
{
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), FoundActors);
	WallArray.Empty();
	const int maxNumber = 8; // from the EDoorOrientation enum 
	for (int i = 0; i < maxNumber; i++)
	{
		WallArray.Add(FWallArr{});
	}
	for (size_t i = 0; i < FoundActors.Num(); i++)
	{
		AWall* wall = Cast<AWall>(FoundActors[i]);
		if (wall->GetOrientation() == EWindOrientation::UnKnown)
		{
			UE_LOG(LogTemp, Warning, TEXT("orentation was unknown %s"), *wall->GetName());
			continue;
		}
		WallArray[int(wall->GetOrientation())].WallArray.Add(wall);
	}
}

void ARoomController::FindSpawner()
{
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundActors);
	if (FoundActors.Num() != 0)
	{
		Spawner = Cast<ASpawner>(FoundActors[0]);
	}
}

void ARoomController::FindDoors()
{
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);
	int32 amountOfDoors = FoundActors.Num();
	if (amountOfDoors == 0)
	{
		return;
	}
	// sorting on x pos
	Algo::Sort(FoundActors, [](AActor* a1, AActor* a2)
		{
			return a1->GetActorLocation().X > a2->GetActorLocation().X;
		});
	NDoor = Cast<ADoor>(FoundActors[0]);
	SDoor = Cast<ADoor>(FoundActors[amountOfDoors-1]);
	// sorting on y pos
	Algo::Sort(FoundActors, [](AActor* a1, AActor* a2)
		{
			return a1->GetActorLocation().Y > a2->GetActorLocation().Y;
		});
	EDoor = Cast<ADoor>(FoundActors[0]);
	WDoor = Cast<ADoor>(FoundActors[amountOfDoors - 1]);
}