// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "DSWeapon.h"
#include "DSWeaponSwordBase.generated.h"

USTRUCT(BlueprintType)
struct FDSWeaponAnim
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* WeaponAnim;

	UPROPERTY(EditAnywhere)
	float PlayRate;

	FDSWeaponAnim()
		: WeaponAnim(nullptr)
		, PlayRate(1.f)
	{
	}

};

USTRUCT(BlueprintType)
struct FDSWeaponAttackSequence
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDSWeaponAnim AttackAnim;

private:
	UPROPERTY(EditAnywhere)
	float AttackSequenceDuration = 1.f;

	// 공격 시작 후 움직임을 허용하는 시간
	UPROPERTY(EditAnywhere)
	float MoveAllowTimeInterval;

	uint8 bAttacking : 1;
	uint8 bEnableCombo : 1;

public:
	FDSWeaponAttackSequence()
		: AttackAnim()
		, AttackSequenceDuration(1.f)
		, MoveAllowTimeInterval(0.f)
		, bAttacking(false)
		, bEnableCombo(false)
	{
	}

	bool IsAttacking() const { return bAttacking; }
	float GetMoveAllowTimeInterval() const { return MoveAllowTimeInterval; }
	void Attack()
	{
		bAttacking = true;
	}

	void SetEnableCombo(bool bEnable)
	{
		bEnableCombo = bEnable;
	}

	bool CanCombo() const
	{
		return bEnableCombo;
	}

	void Reset()
	{
		bAttacking = false;
		bEnableCombo = false;
	}

};

USTRUCT()
struct FDSAttackHitCheckHelper
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName WeaponTraceStart;

	UPROPERTY(EditAnywhere)
	FName WeaponTraceEnd;

private:
	UPROPERTY(EditAnywhere)
	int32 HitCheckTraceNum;

	UPROPERTY(EditAnywhere)
	float HitCheckInterval;

	TWeakObjectPtr<class ADSWeaponSwordBase> OwnerPrivate;
	TArray<FVector> AttackTraceFootstep;
	FCollisionQueryParams CollisionQuery;
	float ElapsedTime = 0.f;
	uint8 bActive : 1;

public:
	FDSAttackHitCheckHelper()
	{
		bActive = false;
	}

	void Initialize(class ADSWeaponSwordBase* OwnerSwordWeapon);
	void Reset();
	void Update(float DeltaTime);
	void HitCheckStart();
	void HitCheckEnd();
	bool IsInHitCheckProcess() const { return bActive; }

private:
	void UpdateAttackTraceFootstep();
	void HitCheck();

};

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

private:
	void UpdateAttackSequence(float DeltaTime);

public:
	// Called every frame
	virtual bool CanAttack() const override;
	const FDSWeaponAttackSequence* GetAttackSequence(int32 Index) const;
	virtual void TryAttack(uint8 TryAttackType = 1) override;

	// ~ Begin AActor Interface
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	// ~ End AActor Interface

public:
	void RequestHitCheckStart();
	void RequestHitCheckEnd();
	void RequestComboCheckStart();
	void RequestComboCheckEnd();
	virtual void OnAttackEnd() override;

protected:
	virtual bool DoAttack() override;
	virtual void InternalEquipped() override;
	virtual void InternalUnequipped() override;
	void SubscribeWeaponAnimDelegate();
	void OnAttackMontageEnd(class UAnimMontage* AttackAnimMontage, bool bInterrupted);

private:
	FDSWeaponAttackSequence* GetAttackSequence(int32 Index);

public:
	UPROPERTY(EditAnywhere)
	FDSAttackHitCheckHelper AttackHitCheckHelper;

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxCombo = 1;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDSWeaponAttackSequence> AttackSequence;

	uint8 bEnableComboInput : 1;

private:
	FOnMontageEnded OnAttackMontageEnded;

};
