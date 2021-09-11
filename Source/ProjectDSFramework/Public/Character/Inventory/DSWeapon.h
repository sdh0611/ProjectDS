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

USTRUCT()
struct FAttackSequenceReplicateData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int8 SequenceIndex;

	UPROPERTY()
	uint8 AttackType;

	FAttackSequenceReplicateData()
		: SequenceIndex(INDEX_NONE)
		, AttackType(0)
	{
	}

	FAttackSequenceReplicateData(int8 NewSequenceIndex, uint8 NewAttackType)
		: SequenceIndex(NewSequenceIndex)
		, AttackType(NewAttackType)
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
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanAttack() const { return IsEquipped(); }
	virtual bool ShouldResolvePendingAttack() const;
	virtual void TryAttack(uint8 TryAttackType = 1);

protected:
	virtual bool DoAttack() { return false; }
	virtual void InternalUpdateWeapon(float DeltaTime);
	virtual void InternalEquipped() override;
	virtual void InternalUnequipped() override;
	void UpdatePendingAttack();
	void CheckExpiredPendingAttack();

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

	UPROPERTY(Transient)
	int32 CurrentCombo = 0;

	// For simulated proxy
	// TODO : Attack type 정의 ex) 강공격, 약공격, 대쉬 공격 등
	UPROPERTY(Transient, Replicated)
	TArray<FAttackSequenceReplicateData> PendingAttack;

	//UPROPERTY(Transient, Replicated)
	//TArray<uint8> PendingAttack;


};
