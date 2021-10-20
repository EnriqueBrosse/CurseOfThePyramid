// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGeneratorStructs.generated.h"

class UStaticMesh;
class UMaterialInterface;

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Fixed = 0 UMETA(DisplayName = "Fixed"),
	Interchangable = 1   UMETA(DisplayName = "Interchangable"),
	Key = 2   UMETA(DisplayName = "Key"),
	RoomBeforeBoss = 3   UMETA(DisplayName = "RoomBeforeBoss"),
	StartRoom = 4   UMETA(DisplayName = "StartRoom"),
};

USTRUCT(BlueprintType)
struct FWallData 
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		UStaticMesh* Wall = nullptr; 
	UPROPERTY(EditAnywhere)
		UMaterialInterface* BigWallMat;
	UPROPERTY(EditAnywhere)
		UStaticMesh* WallShort = nullptr;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* SmallWallMat;
};

USTRUCT(BlueprintType)
struct FGraphNode
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int NodeID{-1};
	UPROPERTY(EditAnywhere)
		ERoomType RoomType = ERoomType::Key;
	UPROPERTY(EditAnywhere)
		FString RoomName = "";
	bool operator==(const FGraphNode& node)const
	{
		return NodeID == node.NodeID;
	}
	bool operator<(const FGraphNode& node)const
	{
		return NodeID < node.NodeID;
	}

	bool IsValid()const
	{
		return NodeID != -1;
	}

};

USTRUCT(BlueprintType)
struct FConnection
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int From{ -1 };
	UPROPERTY(EditAnywhere)
		int To{ -1 };
	bool operator==(const FConnection& connect)const
	{
		return connect.From == From && connect.To == To;
	}

	bool IsValid()const
	{
		return From != -1 && To != -1;
	}
};

//https://medium.com/swlh/using-custom-c-structs-as-tmap-keys-in-unreal-engine-ca3be6be3fea
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FGraphNode& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FGraphNode& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FGraphNode));
	return Hash;
}
#endif
