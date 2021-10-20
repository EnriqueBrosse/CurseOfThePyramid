#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"

// replace these
#include "../OldEnemies/BaseEnemy_01.h"
#include "../OldEnemies/BaseAiController_01.h"
//#include "../../PlayerController/MainPlayerController.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyManager::BeginPlay()
{
	//TArray<AActor*> playerControllerArray{};
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainPlayerController::StaticClass(), playerControllerArray);
	//if (playerControllerArray.Num() != 0)
	//{
	//	PlayerController = Cast<AMainPlayerController>(playerControllerArray[0]);
	//	PlayerController->OnSwitchWorld.AddDynamic(this, &AEnemyManager::SwitchWorld);
	//}
}

ABaseEnemy_01* AEnemyManager::GetEnemy(TSubclassOf<ABaseEnemy_01> enemyClass)
{
	//get or make the right array
	TArray<ABaseEnemy_01*>* classArray = _enemyMap.Find(enemyClass);
	if (classArray == nullptr)
	{
		_enemyMap.Add(enemyClass, TArray<ABaseEnemy_01*>{});
		classArray = _enemyMap.Find(enemyClass);
	}

	//check if anything usable is in that array
	for (size_t i{ 0 }; i < classArray->Num(); i++)
	{
		ABaseEnemy_01* enemy = (*classArray)[i];
		if (!enemy->GetIsActive())
		{
			enemy->SetIsActive(true);
			return enemy;
		}
	}

	//nothing was found, make a new one
	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (!GetWorld()->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyManager::GetEnemy world is nullptr"));
		return nullptr;
	}

	ABaseEnemy_01* newEnemy = GetWorld()->SpawnActor<ABaseEnemy_01>(enemyClass, spawnParams);
	newEnemy->SetIsActive(true);
	classArray->Add(newEnemy);
	return newEnemy;
	return nullptr;
}

void AEnemyManager::SwitchWorld(bool isSoul)
{
	//TArray<TSubclassOf<ABaseEnemy_01>> keyArray{};
	//_enemyMap.GenerateKeyArray(keyArray);

	//for (size_t key{}; key < keyArray.Num(); ++key)
	//{
	//	TArray<ABaseEnemy_01*> enemyArray{ _enemyMap[keyArray[key]] };

	//	for (size_t enemy{}; enemy < enemyArray.Num(); ++enemy)
	//	{
	//		ABaseEnemy_01* baseEnemy{ enemyArray[enemy] };
	//		if (baseEnemy->GetIsActive())
	//		{
	//			baseEnemy->HandleWorldSwitch(isSoul);
	//		}
	//	}
	//}
}