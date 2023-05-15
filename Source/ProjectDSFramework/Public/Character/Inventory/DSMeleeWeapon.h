// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSWeapon.h"
#include "DSBaseTypes.h"
#include "DSMeleeWeapon.generated.h"

USTRUCT()
struct FPendingAttackData
{
	GENERATED_USTRUCT_BODY()

	EAttackType AttackType;

	FPendingAttackData()
		: AttackType(EAttackType::None)
	{
	}

	FPendingAttackData(EAttackType NewAttackType)
		: AttackType(NewAttackType)
	{
	}

	void Reset()
	{
		AttackType = EAttackType::None;
	}
};

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSMeleeWeapon : public ADSWeapon
{
	GENERATED_BODY()
	
public:
	ADSMeleeWeapon();

protected:
	// ~ Begin AActor Interface
	virtual void BeginPlay() override;
	// ~ End AActor Interface
	virtual void InternalUpdateWeapon(float DeltaTime) override;

public:
	virtual bool CanHandleWeaponActionInput(EWeaponActionInput InWeaponActionInput) const override;
	virtual void HandleWeaponActionInput(EWeaponActionInput InWeaponActionInput) override;

	// ~ Begin AActor Interface
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	// ~ End AActor Interface

protected:
	virtual void InternalEquipped() override;
	virtual void InternalUnequipped() override;
	void TryAttack(EWeaponActionInput InAttackInput);
	void DoAttack();
	void DoGuard();
	void DoWeaponSkill();

	bool IsAttacking() const;


protected:
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<FDSWeaponAnimData> AttackAnims;

	UPROPERTY(EditDefaultsOnly, Category = Guard)
	FDSWeaponAnimData GuardAnim;




private:
	UPROPERTY(Transient) // For debug
	FPendingAttackData PendingAttack;

};
