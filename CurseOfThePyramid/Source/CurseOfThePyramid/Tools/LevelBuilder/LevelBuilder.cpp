// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuilder.h"
#include "../../DataAssets/Dungeon/DA_Level.h"
#include "../../Dungeon/Generation/LevelGeneratorStructs.h"
#include "NodeWrapper.h"
#include "ConnectionWrapper.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelBuilder::ALevelBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	SetRootComponent(StaticMeshComp);
}

void ALevelBuilder::OnConstruction(const FTransform& Transform)
{
	// plane scale is 1000x1000 and a block is 100x100
	const float blockSize = 100 / 1000.f;
	SetActorScale3D(FVector{ Colums * blockSize, Rows * blockSize, 1.f });
}

void ALevelBuilder::MakeLevel()
{
	TArray<AActor*> foundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANodeWrapper::StaticClass(), foundActors);
	TArray<FGraphNode> nodes{};
	nodes.Reserve(foundActors.Num());
	const FVector& center = GetActorLocation();
	// filling in the nodes and the nodes array for the data asset
	for (size_t i = 0; i < foundActors.Num(); i++)
	{
		ANodeWrapper* node = Cast<ANodeWrapper>(foundActors[i]);
		node->NodeID = GetNodeID(foundActors[i]->GetActorLocation(), center);
		FGraphNode graphNode{};
		graphNode.NodeID = node->NodeID;
		graphNode.RoomType = node->GetRoomType();
		graphNode.RoomName = node->GetLevelPath();
		nodes.Add(graphNode);
	}
	foundActors.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConnectionWrapper::StaticClass(), foundActors);
	TArray<FConnection> connections{};
	connections.Reserve(foundActors.Num());
	// filling in the connections and the connections array for the data asset
	for (size_t i = 0; i < foundActors.Num(); i++)
	{
		AConnectionWrapper* connection = Cast<AConnectionWrapper>(foundActors[i]);
		connection->FromNodeID = GetNodeID(foundActors[i]->GetActorLocation(), center);
		connection->ToNodeID = GetOtherNodeId(connection->FromNodeID, connection->Dir);
		FConnection connectionStruct{};
		connectionStruct.From = connection->FromNodeID;
		connectionStruct.To = connection->ToNodeID;
		connections.Add(connectionStruct);
	}
	// seting the level data in the data asset
	if (!LevelToReplace)
	{
		return;
	}
	LevelToReplace->AmountOfColumns = Colums;
	LevelToReplace->AmountOfRows = Rows;
	LevelToReplace->Nodes.Empty();
	LevelToReplace->Nodes = nodes;
	LevelToReplace->Connections.Empty();
	LevelToReplace->Connections = connections;
}

int ALevelBuilder::GetNodeID(const FVector& pos, const FVector& center)
{
	FVector posToCheck{};
	const FVector& bounds = StaticMeshComp->Bounds.BoxExtent;
	posToCheck.X = pos.X - center.X + bounds.X;
	posToCheck.Y = pos.Y - center.Y + bounds.Y;
	const float cellsize = (bounds.X * 2) / Colums;

	const int rowIndex = posToCheck.X / cellsize;
	const int colIndex = posToCheck.Y / cellsize;

	return colIndex + (rowIndex * Rows);
}

int ALevelBuilder::GetOtherNodeId(int fromId, EWindOrientation dir)
{
	switch (dir)
	{
	case EWindOrientation::North:
		return fromId + Rows;
		break;
	case EWindOrientation::NorthEast:
		return fromId + Rows + 1;
		break;
	case EWindOrientation::East:
		return fromId + 1;
		break;
	case EWindOrientation::SouthEast:
		return fromId - Rows + 1;
		break;
	case EWindOrientation::South:
		return fromId - Rows;
		break;
	case EWindOrientation::SouthWest:
		return fromId - Rows - 1;
		break;
	case EWindOrientation::West:
		return fromId - 1;
		break;
	case EWindOrientation::NorthWest:
		return fromId + Rows - 1;
		break;
	default:
		break;
	}
	return INDEX_NONE;
}