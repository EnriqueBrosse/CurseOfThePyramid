#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Poolable.h"
#include "BaseEnemy_01.generated.h"

class USkeletalMeshComponent;
class UDA_EnemyData;
class ABaseAIController_01;
//class AEnemyWeapon;
class UMaterialInstance;

UCLASS()
class CURSEOFTHEPYRAMID_API ABaseEnemy_01 : public ACharacter, public IPoolable
{
	GENERATED_BODY()

public:
	ABaseEnemy_01();
	virtual void Tick(float DeltaTime) override;
	
	void InitializeWithData(UDA_EnemyData* enemyData);
	
	//UFUNCTION(BlueprintCallable)
	//AEnemyWeapon* GetWeapon() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetIsActive(bool bIsActive) override;

	UFUNCTION()
		void HandleWorldSwitch(bool isSoulWorld);
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyHit();

	UFUNCTION(BlueprintImplementableEvent)
		void DataInitialized();
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		ABaseAIController_01* UsedController;

	UPROPERTY()
		USkeletalMeshComponent* InterchangeableMesh1;
	UPROPERTY()
		USkeletalMeshComponent* InterchangeableMesh2;

	//UPROPERTY()
	//	AEnemyWeapon* Weapon;

	UPROPERTY(EditAnywhere)
		FName MeleeSocketName;

	UPROPERTY()
		TArray<UMaterialInstance*> BaseMaterials;
	UPROPERTY()
		TArray<UMaterialInstance*> InterchangeableMaterials1;
	UPROPERTY()
		TArray<UMaterialInstance*> InterchangeableMaterials2;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* WrongWorldMaterial;

private:
	UFUNCTION()
		void SetMaterials(const TArray<UMaterialInstance*>& materials, USkeletalMeshComponent* meshComp);

	UFUNCTION()
		void ChangeToWrongWorldMaterials();
	UFUNCTION()
		void ChangeToHomeWorldMaterials();
};
