#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWindOrientation : uint8
{
	North = 0 UMETA(DisplayName = "North"),
	NorthEast = 1 UMETA(DisplayName = "NorthEast"),
	East = 2 UMETA(DisplayName = "East"),
	SouthEast = 3 UMETA(DisplayName = "SouthEast"),
	South = 4 UMETA(DisplayName = "South"),
	SouthWest = 5 UMETA(DisplayName = "SouthWest"),
	West = 6 UMETA(DisplayName = "West"),
	NorthWest = 7 UMETA(DisplayName = "NorthWest"),
	UnKnown = 8 UMETA(DisplayName = "UnKnown")
};
