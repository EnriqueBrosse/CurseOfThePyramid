#include "BaseEnemy_01.h"
#include "BehaviorTree/BehaviorTree.h"

#include "DA_EnemyData.h"
#include "BaseAiController_01.h"
//#include "../../PlayerController/MainPlayerController.h"
//#include "../../Actors/Projectiles/BaseProjectile.h"
//#include "../../Actors/Weapons/EnemyWeapon.h"

ABaseEnemy_01::ABaseEnemy_01()
	: UsedController{}
	, InterchangeableMesh1{}
	, InterchangeableMesh2{}
	//, Weapon{}
	, MeleeSocketName{"MeleeSocket"}
	, BaseMaterials{}
	, InterchangeableMaterials1{}
	, InterchangeableMaterials2{}
{
 	PrimaryActorTick.bCanEverTick = true;

	InterchangeableMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>("Interchangable1");
	InterchangeableMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>("Interchangable2");

	InterchangeableMesh1->SetupAttachment(GetMesh());
	InterchangeableMesh2->SetupAttachment(GetMesh());

	InterchangeableMesh1->SetMasterPoseComponent(GetMesh());
	InterchangeableMesh2->SetMasterPoseComponent(GetMesh());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABaseAIController_01::StaticClass();

	Tags.Add("Enemy");
}

void ABaseEnemy_01::BeginPlay()
{
	Super::BeginPlay();
	
	UsedController = Cast<ABaseAIController_01>(GetController());
}

void ABaseEnemy_01::InitializeWithData(UDA_EnemyData* enemyData)
{
	//BASE MESH
	USkeletalMeshComponent* meshComp = GetMesh();	
	meshComp->SetSkeletalMesh(enemyData->BaseMesh);
	BaseMaterials = enemyData->BaseMaterials;
	SetMaterials(BaseMaterials, meshComp);

	//INTERCHANGEABLE MESH 1
	InterchangeableMesh1->SetSkeletalMesh(enemyData->InterchangeableMesh1);
	InterchangeableMaterials1 = enemyData->InterchangeableMaterials1;
	SetMaterials(InterchangeableMaterials1, InterchangeableMesh1);

	//INTERCHANGEABLE MESH 2
	InterchangeableMesh2->SetSkeletalMesh(enemyData->InterchangeableMesh2);
	InterchangeableMaterials2 = enemyData->InterchangeableMaterials2;
	SetMaterials(InterchangeableMaterials1, InterchangeableMesh1);

	//WEAPON
	//if (Weapon->IsValidLowLevelFast()) Weapon->Destroy(); // Clean up any old weapons
	
	//if (enemyData->Type == EType::Melee)
	//{
	//	FActorSpawnParameters spawnParameters;
	//	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	spawnParameters.Owner = this;
	//	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,true };
	//
	//	FTransform tranformWeapon = GetMesh()->GetSocketTransform(MeleeSocketName, ERelativeTransformSpace::RTS_ParentBoneSpace);
	//	Weapon = GetWorld()->SpawnActor<AEnemyWeapon>(enemyData->Weapon, tranformWeapon, spawnParameters);
	//	Weapon->AttachToComponent(GetMesh(), attachmentRulesWeapon, MeleeSocketName);
	//}

	//ANIMATION
	meshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	meshComp->SetAnimInstanceClass(enemyData->AnimationBlueprint);
	DataInitialized();
}

//AEnemyWeapon* ABaseEnemy_01::GetWeapon() const
//{
//	return Weapon;
//}

void ABaseEnemy_01::SetMaterials(const TArray<UMaterialInstance*>& materials, USkeletalMeshComponent* meshComp)
{
	int maxMaterials{};
	const int meshMaterialSlotNr{ meshComp->GetNumMaterials() };
	const int givenMaterialSlotNr{ materials.Num() };

	if (meshMaterialSlotNr > givenMaterialSlotNr) maxMaterials = givenMaterialSlotNr;
	else maxMaterials = meshMaterialSlotNr;

	for (int i{ 0 }; i < maxMaterials; ++i)
	{
		meshComp->SetMaterial(i, materials[i]);
	}
}

void ABaseEnemy_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

//TODO: it might be an idea to make a different TakeDamage function, that just takes the damage, world, and type of damage.
// deriving both enemies and player from a single (controller) base class might be a way to get past some casting, but as long
// as there arent too many this should be fine anyway.
float ABaseEnemy_01::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	bool isPhysicalDamage{};
	bool isNormalWorldDamage{};

	//AMainPlayerController* playerController = Cast<AMainPlayerController>(EventInstigator);
	//if (playerController->IsValidLowLevelFast())
	//{
	//	if (playerController->bIsSoul) isNormalWorldDamage = false;
	//	else isNormalWorldDamage = true;
	//}

	//ABaseProjectile* projectile{ Cast<ABaseProjectile>(DamageCauser)};
	//if (projectile->IsValidLowLevelFast()) isPhysicalDamage = false;
	//else isPhysicalDamage = true;

	DamageAmount = UsedController->ApplyDamage(DamageAmount, isPhysicalDamage, isNormalWorldDamage);
	EnemyHit();
	return DamageAmount;
}

void ABaseEnemy_01::SetIsActive(bool isActive)
{
	bIsActive = isActive;

	if (isActive)
	{
		UsedController->SetActorTickEnabled(true);
		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);

		//if (Weapon)
		//{
		//	Weapon->SetActorTickEnabled(true);
		//	Weapon->SetActorHiddenInGame(false);
		//	Weapon->SetActorEnableCollision(true);
		//}
	}
	else
	{
		UsedController->SetActorTickEnabled(false);
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		//if (Weapon)
		//{
		//	Weapon->SetActorTickEnabled(false);
		//	Weapon->SetActorHiddenInGame(true);
		//	Weapon->SetActorEnableCollision(false);
		//}
	}
}

void ABaseEnemy_01::ChangeToWrongWorldMaterials()
{
	USkeletalMeshComponent* meshComp{ GetMesh() };

	for (int i{}; i < meshComp->GetNumMaterials(); ++i)
	{
		meshComp->SetMaterial(i, WrongWorldMaterial);
	}

	for (int i{}; i < InterchangeableMesh1->GetNumMaterials(); ++i)
	{
		InterchangeableMesh1->SetMaterial(i, WrongWorldMaterial);
	}

	for (int i{}; i < InterchangeableMesh2->GetNumMaterials(); ++i)
	{
		InterchangeableMesh2->SetMaterial(i, WrongWorldMaterial);
	}
}

void ABaseEnemy_01::ChangeToHomeWorldMaterials()
{
	SetMaterials(BaseMaterials, GetMesh());
	SetMaterials(InterchangeableMaterials1, InterchangeableMesh1);
	SetMaterials(InterchangeableMaterials2, InterchangeableMesh2);
}

void ABaseEnemy_01::HandleWorldSwitch(bool isSoulWorld)
{
	const bool isSoulHome{ UsedController->IsSoulHomeWorld() };

	if ((isSoulWorld && isSoulHome) ||
		(!isSoulWorld && !isSoulHome))
	{
		ChangeToHomeWorldMaterials();
	}
	else if ((isSoulWorld && !isSoulHome) ||
			(!isSoulWorld && isSoulHome))
	{
		ChangeToWrongWorldMaterials();
	}
}