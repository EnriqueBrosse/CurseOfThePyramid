#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_EnemyData.generated.h"

class UBehaviorTree;
class UBlackboardData;

class USkeletalMesh;
class UMaterialInstance;
class UAnimBlueprint;

//class ABaseProjectile;
class AEnemyWeapon;

UENUM(BlueprintType)
enum EType
{
	Melee	UMETA(DisplayName = "Melee"),
	Ranged	UMETA(DisplayName = "Ranged"),
};

UCLASS()
class CURSEOFTHEPYRAMID_API UDA_EnemyData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type")
		TEnumAsByte<EType> Type;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	//	TSubclassOf<ABaseProjectile> Projectile;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	//	TSubclassOf<AEnemyWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float BaseHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthGrowthrate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float BasePhysicalArmor = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float MaxPhysicalArmor = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float PhysicalArmorGrowthrate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float BaseMagicalArmor = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float MaxMagicalArmor = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float MagicalArmorGrowhtrate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		bool bHasShield = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float BaseShieldHealth = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float MaxShieldHealth = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float ShieldHealthGrowthrate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float BasePhysicalDamage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MaxPhysicalDamage = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float PhysicalDamageGrowthrate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float BaseMagicalDamage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MaxMagicalDamage = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MagicalDamageGrowthrate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float BaseAttackSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float MaxAttackSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float AttackSpeedGrowthrate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float BaseMovementSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float MaxMovementSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float MovementSpeedGrowthrate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float BaseGoldDropAmount = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float MaxGoldDropAmount = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float GoldDropAmountGrowthrate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float BaseGoldDropChance = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float MaxGoldDropChance = 0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
		float GoldDropChanceGrowthrate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
		bool bIsFromNormalWorld = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, Category = "Visuals")
		USkeletalMesh* BaseMesh;
	UPROPERTY(EditAnywhere, Category = "Visuals")
		USkeletalMesh* InterchangeableMesh1;
	UPROPERTY(EditAnywhere, Category = "Visuals")
		USkeletalMesh* InterchangeableMesh2;
	UPROPERTY(EditAnywhere, Category = "Visuals")
		TArray<UMaterialInstance*> BaseMaterials;
	UPROPERTY(EditAnywhere, Category = "Visuals")
		TArray<UMaterialInstance*> InterchangeableMaterials1;
	UPROPERTY(EditAnywhere, Category = "Visuals")
		TArray<UMaterialInstance*> InterchangeableMaterials2;
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
		TSubclassOf<class UAnimInstance> AnimationBlueprint;
};
