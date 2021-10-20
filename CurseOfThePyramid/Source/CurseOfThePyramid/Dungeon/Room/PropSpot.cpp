// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSpot.h"
#include "Components/BoxComponent.h"

// Sets default values
APropSpot::APropSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(root);
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
