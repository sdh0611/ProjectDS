// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSEquipment.h"
#include "DSWeapon.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponAnimInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName AnimName;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* SlotAnim;

	UPROPERTY(EditAnywhere)
	float PlayRate;

	FWeaponAnimInfo()
		: AnimName(NAME_None)
		, SlotAnim(nullptr)
		, PlayRate(1.f)
	{
	}
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanAttack() const { return IsEquipped(); }
	virtual void DoAttack() { }

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

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_WeaponArmed)
	uint8 bWeaponArmed: 1;

};
