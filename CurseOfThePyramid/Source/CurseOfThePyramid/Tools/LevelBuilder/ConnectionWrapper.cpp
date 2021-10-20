// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionWrapper.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AConnectionWrapper::AConnectionWrapper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	SetRootComponent(root);
	StaticMeshComp->SetupAttachment(RootComponent);
}

void AConnectionWrapper::OnConstruction(const FTransform& Transform)
{
	SetActorRotation(FQuat(FRotator{ 0,int(Dir) * 45.f,0 }));
}
