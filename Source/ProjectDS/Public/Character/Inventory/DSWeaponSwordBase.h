// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "DSWeapon.h"
#include "DSWeaponSwordBase.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTDS_API ADSWeaponSwordBase : public ADSWeapon
{
	GENERATED_BODY()
	
public:
	ADSWeaponSwordBase();

protected:
	// ~ Begin AActor Interface
	virtual void BeginPlay() override;
	// ~ End AActor Interface
	virtual void InternalUpdateWeapon(float DeltaTime) override;

public:
	virtual bool CanAttack(EAttackInputType TryAttackType) const override;
	virtual void TryAttack(EAttackInputType TryAttackType) override;

	// ~ Begin AActor Interface
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	// ~ End AActor Interface

protected:
	virtual void InternalEquipped() override;
	virtual void InternalUnequipped() override;

};
