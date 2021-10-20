// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeWrapper.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ANodeWrapper::ANodeWrapper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	SetRootComponent(StaticMeshComp);
}

void ANodeWrapper::OnConstruction(const FTransform& Transform)
{
	UMaterialInstance* materialInstance = nullptr;
	switch (RoomType)
	{
	case ERoomType::Fixed:
		materialInstance = FixedLevelMat;
		break;
	case ERoomType::Interchangable:
		materialInstance = InterchangableLevelMat;
		break;
	case ERoomType::Key:
		materialInstance = KeyLevelMat;
		break;
	case ERoomType::RoomBeforeBoss:
		materialInstance = RoomBeforeBossLevelMat;
		break;
	case ERoomType::StartRoom:
		materialInstance = StartLevelMat;
		break;
	default:
		break;
	}
	StaticMeshComp->SetStaticMesh(StaticMesh);
	StaticMeshComp->SetMaterial(0,materialInstance);
}
