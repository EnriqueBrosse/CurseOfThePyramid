// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "Graph.h"
#include "../Room/RoomController.h"
#include "../../DataAssets/Dungeon/DA_Level.h"
#include "../../Managers/Spawner.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	Graph = NewObject<UGraph>();
	MakeDungeonGrid();
}

void ADungeonGenerator::MakeDungeonGrid()
{
	if (!Level->IsValidLowLevelFast())
	{
		return;
	}
	AmountOfRows = Level->AmountOfRows;
	AmountOfColumns = Level->AmountOfColumns;
	const TArray<FGraphNode>& nodes = Level->Nodes;
	for (size_t i = 0; i < nodes.Num(); i++)
	{
		Graph->AddNode(nodes[i].NodeID);
	}
	const TArray<FConnection>& connections = Level->Connections;
	for (size_t i = 0; i < connections.Num(); i++)
	{
		Graph->MakeConnection(connections[i].From, connections[i].To);
	}
	Graph->SetColumnSize(AmountOfColumns);
	Graph->SetRowSize(AmountOfRows);

	FRandomStream rand(0);

	// getting the offset 
	for (size_t i = 0; i < nodes.Num(); i++)
	{
		if (nodes[i].RoomType == ERoomType::StartRoom)
		{
			LocationOffset = FVector(GetNodePosition(nodes[i].NodeID));
			LocationOffset.X = -LocationOffset.X;
			LocationOffset.Y = -LocationOffset.Y;
			break;
		}
	}

	// streaming in the level
	for (size_t i = 0; i < nodes.Num(); i++)
	{
		FVector newPosition = GetNodePosition(nodes[i].NodeID);
		// add rotation -> do this later
		int32 levelNum = rand.RandRange(0, FlexibleLevels.Num() - 1);
		bool success = false;
		ULevelStreamingDynamic* dynLevel = nullptr;
		switch (nodes[i].RoomType)
		{
		case ERoomType::Fixed:
			dynLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), FlexibleLevels[levelNum], newPosition, FRotator{ 0 }, success);
			StreamedLevels.Add(dynLevel);
			dynLevel->OnLevelLoaded.AddDynamic(this, &ADungeonGenerator::OnLevelLoaded);
			break;
		case ERoomType::Interchangable:
			dynLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), FlexibleLevels[levelNum], newPosition, FRotator{ 0 }, success);
			StreamedLevels.Add(dynLevel);
			dynLevel->OnLevelLoaded.AddDynamic(this, &ADungeonGenerator::OnLevelLoaded);
			break;
		case ERoomType::Key:
			dynLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), FlexibleLevels[levelNum], newPosition, FRotator{ 0 }, success);
			StreamedLevels.Add(dynLevel);
			dynLevel->OnLevelLoaded.AddDynamic(this, &ADungeonGenerator::OnLevelLoaded); 
			break;
		case ERoomType::RoomBeforeBoss:
			dynLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), FlexibleLevels[levelNum], newPosition, FRotator{ 0 }, success);
			StreamedLevels.Add(dynLevel);
			dynLevel->OnLevelLoaded.AddDynamic(this, &ADungeonGenerator::OnLevelLoaded); 
			break;
		case ERoomType::StartRoom:
			dynLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), FlexibleLevels[levelNum], newPosition, FRotator{ 0 }, success);
			StreamedLevels.Add(dynLevel);
			dynLevel->OnLevelLoaded.AddDynamic(this, &ADungeonGenerator::OnLevelLoaded);
			break;
		default:
			break;
		}
	}
}

FVector ADungeonGenerator::GetNodePosition(int nodeId)
{
	FVector pos = GetActorLocation();
	const int rowIndex = nodeId / AmountOfRows;
	const int colIndex = nodeId % AmountOfRows;
	pos.X += rowIndex * GridSize + LocationOffset.X;
	pos.Y += colIndex * GridSize + LocationOffset.Y;
	return pos;
}

void ADungeonGenerator::AddRoomController()
{
	TArray<AActor*> outActors{};
	TArray<AActor*> actorsNotFound{};
	UGameplayStatics::GetAllActorsOfClass(this, ARoomController::StaticClass(), outActors);
	for (size_t i = 0; i < outActors.Num(); i++)
	{
		ARoomController* roomCon = Cast<ARoomController>(outActors[i]);
		if (RoomControllers.Find(roomCon) == INDEX_NONE)
		{
			RoomControllers.Add(roomCon);
			const FVector& loc = roomCon->GetTargetLocation();
			int rowIndex = (loc.X - LocationOffset.X) / GridSize;
			int colIndex = (loc.Y - LocationOffset.Y) / GridSize;
			//TODO: replace the 0 with the level rotation
			roomCon->SetLevelData(colIndex + rowIndex * AmountOfRows, Graph, 0, this);
		}
	}
}

void ADungeonGenerator::OnLevelLoaded()
{
	FTimerHandle TimerHandle{};
	GetWorldTimerManager().SetTimerForNextTick(this, &ADungeonGenerator::AddRoomController);
}

void ADungeonGenerator::LoadAndUnloadLevels(int nodeTo)
{
}

void ADungeonGenerator::TeleportPlayer(int nodeFrom, int nodeTo, AActor* player)
{
	ARoomController* controller = GetRoomControllerFromID(nodeTo);
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetRoomControllerFromID returned nullptr"));
		return;
	}
	LoadAndUnloadLevels(nodeTo);
	const FVector& loc = controller->GetTeleportTransformFromDoor(nodeFrom).GetLocation();
	const FVector& playerLoc = player->GetActorLocation();
	if (playerLoc.Equals(loc))
	{
		UE_LOG(LogTemp, Warning, TEXT("player will return to original node"));
	}
	player->SetActorLocation(loc, false, nullptr, ETeleportType::TeleportPhysics);
	if (controller->GetSpawner())
	{
		controller->GetSpawner()->StartSpawning();
	}
}

ARoomController* ADungeonGenerator::GetRoomControllerFromID(int nodeId)const
{
	for (size_t i = 0; i < RoomControllers.Num(); i++)
	{
		if (RoomControllers[i]->GetId() == nodeId)
		{
			return RoomControllers[i];
		}
	}
	return nullptr;
}