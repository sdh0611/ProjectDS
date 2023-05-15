// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSEquipment.h"
#include "DSBaseTypes.h"
#include "DSWeaponAction.h"
#include "DSWeapon.generated.h"

class UAnimMontage;
class UDSWeaponAction;


USTRUCT()
struct FDSWeaponActionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	EWeaponActionInput WeaponActionInput;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "WeaponAction")
	UDSWeaponAction* WeaponAction;
};


/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSWeapon : public ADSEquipment
{
	GENERATED_BODY()
	
public:
	ADSWeapon();

protected:
	virtual void BeginPlay() override;

public:
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/**
	 * Called on the actor right before replication occurs.
	 * Only called on Server, and for autonomous proxies if recording a Client Replay.
	 */
	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanHandleWeaponActionInput(EWeaponActionInput InWeaponActionInput) const { return IsEquipped(); }
	virtual void HandleWeaponActionInput(EWeaponActionInput InWeaponActionInput);

protected:
	virtual void InternalUpdateWeapon(float DeltaTime);
	virtual void InternalEquipped() override;
	virtual void InternalUnequipped() override;

public:
	void WeaponArmed();
	void WeaponUnarmed(bool bPlayAnim = true);

public:
	void SetWeaponArmed(bool bIsArmed);
	bool IsWeaponArmed() const { return bWeaponArmed; }
	UDSWeaponAction* GetWeaponAction(EWeaponActionInput InInputType) const;

protected:
	UFUNCTION()
	virtual void OnRep_WeaponArmed();

public:
	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketNameOnDeactivated;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* UnequipAnim;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponAction")
	TArray<FDSWeaponActionData> WeaponActions;

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_WeaponArmed)
	uint8 bWeaponArmed: 1;
};
