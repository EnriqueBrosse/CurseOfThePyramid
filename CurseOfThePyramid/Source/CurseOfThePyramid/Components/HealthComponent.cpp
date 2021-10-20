// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::Heal(float health)
{
	// dummy proofing the arguments
	const float checkedHealth = UKismetMathLibrary::Abs(health);
	if (CurrentHealth < 0.f)
	{
		return;
	}

	CurrentHealth += checkedHealth;
}

// check the argument
// check if death
// take damage 
void UHealthComponent::Damage(float damage)
{
	// dummy proofing the arguments
	const float checkedHealth = UKismetMathLibrary::Abs(damage);
	if (CurrentHealth < 0.f)
	{
		return;
	}

	CurrentHealth -= checkedHealth;

	UE_LOG(LogTemp, Display, TEXT(" %s took %f damage and has %f health"), *GetOwner()->GetName(), damage, CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		OnDeath.Broadcast();
	}
}


void UHealthComponent::SetMaxHealth(float maxHealth)
{
	MaxHealth = maxHealth;
	CurrentHealth = maxHealth;
}
