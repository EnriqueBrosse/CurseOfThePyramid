#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DA_EnemyData.h"
#include "BaseAIController_01.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

class ABaseEnemy_01;
class UHealthComponent;

class AMainPlayerController;
class AProjectileManager;

UCLASS()
class CURSEOFTHEPYRAMID_API ABaseAIController_01 : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController_01();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
		void SetLevel(int level);

	UFUNCTION(BlueprintCallable)
		void OnSpawn(UDA_EnemyData* enemyData);

	UFUNCTION(BlueprintCallable)
		void SetIsAttacking(bool isAttacking);

	UFUNCTION(BlueprintCallable)
		bool IsAttacking() const;

	UFUNCTION()
		float ApplyDamage(float damageAmount, bool bIsPhysical, bool bFromNormalWorld);

	UFUNCTION()
		bool IsSoulHomeWorld() const;

	UFUNCTION(BlueprintCallable)
		TEnumAsByte<EType> GetType() const;

	UFUNCTION(BlueprintCallable)
		void ShootProjectile();
	FORCEINLINE UHealthComponent* GetHealthComponent()const { return HealthComponent; }
protected:
	virtual void BeginPlay() override;

	//---COMPONENTS---//
	UPROPERTY()
		UBehaviorTreeComponent* BTComponent;
	UPROPERTY()
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
		UHealthComponent* HealthComponent;
 	
	UPROPERTY()
		ABaseEnemy_01* ControlledCharacter;

	//---ANIMATION---//
	UPROPERTY(BlueprintReadWrite, Category = "AnimationData")
		bool bIsAttacking;

	//---STATS---//
	UPROPERTY(VisibleAnywhere, Category="Defense")
		float PhysicalArmor;
	UPROPERTY(VisibleAnywhere, Category = "Defense")
		float MagicalArmor;
	UPROPERTY(VisibleAnywhere, Category = "Defense")
		bool bHasShield;
	UPROPERTY(VisibleAnywhere, Category = "Defense")
		float ShieldHealth;
	
	UPROPERTY(VisibleAnywhere, Category = "Damage")
		float PhysicalDamage;
	UPROPERTY(VisibleAnywhere, Category = "Damage")
		float MagicalDamage;
	UPROPERTY(VisibleAnywhere, Category = "Damage")
		float AttackSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Gold")
		float GoldDropAmount;
	UPROPERTY(VisibleAnywhere, Category = "Gold")
		float GoldDropChance;

	UPROPERTY(VisibleAnywhere, Category = "World")
		bool bIsFromNormalWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="World")
		float WorldResistance;

	UPROPERTY()
		TEnumAsByte<EType> Type;

	UFUNCTION()
		float GetResultValue(float baseValue, float maxValue, float growthrate);

	UFUNCTION()
		void HandleDeath();

	//---ATTACK SETUP---///
	//UPROPERTY(VisibleAnywhere, Category = "Settings")
	//	AProjectileManager* ProjectileManager;

	//UPROPERTY(VisibleAnywhere, Category = "Settings")
	//	TSubclassOf<ABaseProjectile> ProjectileClass;

private:
	UPROPERTY(VisibleAnywhere)
		int Level;

	//UPROPERTY(VisibleAnywhere)
	//	AMainPlayerController* PlayerController;
};
