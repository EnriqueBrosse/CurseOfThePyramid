// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "RoomController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(sceneComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TeleportOverlap = CreateDefaultSubobject<UBoxComponent>("TeleportOverlap");
	TeleportOverlap->SetupAttachment(RootComponent);
	TeleportOverlap->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxBeginOverlap);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick(this, &ADoor::SetTickOff);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADoor::MoveDoor(bool open)
{
	bIsOpen = open;
	//TODO: replace this with an opening/closing animation
	if (open && bIsActive)
	{
		Mesh->SetRelativeLocation(OpenedLocation);
		//SetActorTickEnabled(true);
	}
	else
	{
		Mesh->SetRelativeLocation(ClosedLocation);
		//SetActorTickEnabled(true);
	}
}

void ADoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") || !bIsOpen || !bIsActive)
	{
		return;
	}
	RoomController->TeleportPlayer(ToConnectionId, OtherActor);
}
void ADoor::SetTickOff()
{
	SetActorTickEnabled(false);
}