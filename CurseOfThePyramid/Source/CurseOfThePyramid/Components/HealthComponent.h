// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHEPYRAMID_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	// Called every frame
	FOnDeath OnDeath;
	void SetMaxHealth(float maxHealth);
	void Heal(float health);
	void Damage(float damage);

	bool GetIsDead() const { return CurrentHealth <= 0; }

	UFUNCTION(BlueprintCallable)
		float GetHealthPercentage()const { return CurrentHealth / MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float MaxHealth;
	float CurrentHealth;

};
