#include "BaseAIController_01.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BaseEnemy_01.h"
#include "../Components/HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "../../../Actors/Weapons/EnemyWeapon.h"

#include "Kismet/GameplayStatics.h"
//#include "../../../PlayerController/MainPlayerController.h"
//#include "../../BaseCharacter.h"
//
//#include "../../../Actors/Projectiles/BaseProjectile.h"
//#include "../../../Actors/Managers/ProjectileManager.h"

ABaseAIController_01::ABaseAIController_01()
	: BTComponent{}
	, BlackboardComponent{}
	, ControlledCharacter{}
	, bIsAttacking{}
	, PhysicalArmor{}
	, MagicalArmor{}
	, bHasShield{}
	, ShieldHealth{}
	, PhysicalDamage{}
	, MagicalDamage{}
	, AttackSpeed{}
	, GoldDropAmount{}
	, GoldDropChance{}
	, bIsFromNormalWorld{}
	, WorldResistance{}
	, Level{ 1 }
	//, PlayerController{}
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void ABaseAIController_01::BeginPlay()
{
	Super::BeginPlay(); // dumdum me forgot this and caused this whole class to get stuck ._.

	HealthComponent->OnDeath.AddDynamic(this, &ABaseAIController_01::HandleDeath);

	//TArray<AActor*> projectileManagerArray{};
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjectileManager::StaticClass(), projectileManagerArray);
	//if (projectileManagerArray.Num() != 0)
	//{
	//	ProjectileManager = Cast<AProjectileManager>(projectileManagerArray[0]);
	//	return;
	//}
	//ProjectileManager = GetWorld()->SpawnActor<AProjectileManager>();
}

void ABaseAIController_01::OnSpawn(UDA_EnemyData* enemyData)
{
	ControlledCharacter = Cast<ABaseEnemy_01>(GetCharacter());
	//PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!enemyData->IsValidLowLevelFast() || !BlackboardComponent->IsValidLowLevelFast()
		|| !BTComponent->IsValidLowLevelFast() || !ControlledCharacter->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("OnSpawn in BaseAiController_01 failed!"));
		return;
	}

	ControlledCharacter->SetIsActive(true);
	
	//ProjectileClass = enemyData->Projectile;

	HealthComponent->SetMaxHealth(GetResultValue(enemyData->BaseHealth, enemyData->MaxHealth, enemyData->HealthGrowthrate));

	PhysicalArmor = GetResultValue(enemyData->BasePhysicalArmor, enemyData->MaxPhysicalArmor, enemyData->PhysicalArmorGrowthrate);
	MagicalArmor = GetResultValue(enemyData->BaseMagicalArmor, enemyData->MaxMagicalArmor, enemyData->MaxMagicalArmor);
	bHasShield = enemyData->bHasShield;
	ShieldHealth = GetResultValue(enemyData->BaseShieldHealth, enemyData->MaxShieldHealth, enemyData->ShieldHealthGrowthrate);

	PhysicalDamage = GetResultValue(enemyData->BasePhysicalDamage, enemyData->MaxPhysicalDamage, enemyData->PhysicalDamageGrowthrate);
	MagicalDamage = GetResultValue(enemyData->BaseMagicalDamage, enemyData->MaxMagicalDamage, enemyData->MagicalDamageGrowthrate);

	AttackSpeed = GetResultValue(enemyData->BaseAttackSpeed, enemyData->MaxAttackSpeed, enemyData->AttackSpeedGrowthrate);
	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = GetResultValue(enemyData->BaseMovementSpeed, enemyData->MaxMovementSpeed, enemyData->MovementSpeedGrowthrate);
	
	GoldDropAmount = GetResultValue(enemyData->BaseGoldDropAmount, enemyData->MaxGoldDropAmount, enemyData->GoldDropAmountGrowthrate);
	GoldDropChance = GetResultValue(enemyData->BaseGoldDropChance, enemyData->MaxGoldDropChance, enemyData->GoldDropChanceGrowthrate);

	bIsFromNormalWorld = enemyData->bIsFromNormalWorld;

	Type = enemyData->Type;
	
	ControlledCharacter->InitializeWithData(enemyData);
	//ControlledCharacter->HandleWorldSwitch(PlayerController->bIsSoul);


	if (Type == EType::Melee)
	{
		//ControlledCharacter->GetWeapon()->SetBaseDamage(PhysicalDamage);
	}

	BlackboardComponent->InitializeBlackboard(*enemyData->BehaviorTree->BlackboardAsset);
	BTComponent->StartTree(*enemyData->BehaviorTree);
	
}

float ABaseAIController_01::ApplyDamage(float damageAmount, bool bIsPhysical, bool bFromNormalWorld)
{
	//SHIELD
	if (bHasShield)
	{
		if ((bIsFromNormalWorld && bFromNormalWorld) ||
			!bIsFromNormalWorld && !bFromNormalWorld)
		{
			//DAMAGE NEGATED
			return 0.f;
		}
		else
		{
			//DAMAGE SHIELD
			ShieldHealth -= damageAmount;
			if (ShieldHealth <= 0.f) bHasShield = false;
			return damageAmount;
		}
	}

	//WORLD
	if ((!bIsFromNormalWorld && bFromNormalWorld) ||
		bIsFromNormalWorld && !bFromNormalWorld)
	{
		damageAmount -= damageAmount * WorldResistance;
	}

	//ARMOR
	if (bIsPhysical)
	{
		damageAmount -= damageAmount * (PhysicalArmor / 100.f);
	}
	else
	{
		damageAmount -= damageAmount * (MagicalArmor / 100.f);
	}
	
	if (damageAmount < 0.f) damageAmount = 0.f;

	HealthComponent->Damage(damageAmount);
	return damageAmount;
}

bool ABaseAIController_01::IsSoulHomeWorld() const
{
	return !bIsFromNormalWorld;
}

void ABaseAIController_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector playerLocation{ PlayerController->GetCurrentPlayer()->GetActorLocation() };
	//BlackboardComponent->SetValueAsVector("Target", playerLocation);
}

void ABaseAIController_01::SetLevel(int level)
{
	Level = level;
}

float ABaseAIController_01::GetResultValue(float baseValue, float maxValue, float growthrate)
{
	const float cap = (maxValue - baseValue) * 2;
	const float rate = 1 - growthrate;
	const float correction = maxValue - 2 * baseValue;

	return (cap / (1 + UKismetMathLibrary::Exp(rate * Level))) - correction;
}

void ABaseAIController_01::HandleDeath()
{
	ControlledCharacter->SetIsActive(false);
	// reset for the spawners
	HealthComponent->OnDeath.Clear();
	HealthComponent->OnDeath.AddDynamic(this, &ABaseAIController_01::HandleDeath);
}

void ABaseAIController_01::SetIsAttacking(bool isAttacking)
{
	bIsAttacking = isAttacking;
}

bool ABaseAIController_01::IsAttacking() const
{
	return bIsAttacking;
}

TEnumAsByte<EType> ABaseAIController_01::GetType() const
{
	return Type;
}

void ABaseAIController_01::ShootProjectile()
{
	//spawn projectile at socket location
	//set the rotation to fly horizontally
	//set damage

	if (HealthComponent->GetIsDead()) return;

	//ABaseProjectile* proj = ProjectileManager->GetProjectile(ProjectileClass);
	FQuat rotation{};
	FVector location{};

	ControlledCharacter->GetMesh()->GetSocketWorldLocationAndRotation("RangedSocket", location, rotation);

	FVector forward{ rotation.GetForwardVector() };
	forward.Z = 0.f;

	//proj->Damage = MagicalDamage;
	//proj->Shoot(forward, location);
}