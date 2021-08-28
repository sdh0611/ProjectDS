// All rights reserve SDH (2021 ~ )

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

	UPROPERTY(EditAnywhere)
	float ComboCheckStart = 0.f;

	UPROPERTY(EditAnywhere)
	float ComboCheckEnd = 0.f;

	// 공격 시작 후 Hit check 타이밍
	UPROPERTY(EditAnywhere)
	float HitCheckTime;

	// 공격 시작 후 움직임을 허용하는 시간
	UPROPERTY(EditAnywhere)
	float MoveAllowTimeInterval;

	float ElapsedTime = 0.f;
	uint8 bAttacking : 1;

public:
	FDSWeaponAttackSequence()
		: AttackAnim()
		, AttackSequenceDuration(1.f)
		, ComboCheckStart(0.f)
		, ComboCheckEnd(0.f)
		, HitCheckTime(0.f)
		, MoveAllowTimeInterval(0.f)
		, ElapsedTime(0.f)
		, bAttacking(false)
	{
	}

	bool IsAttacking() const { return bAttacking; }
	bool IsSequenceEnd() const { return ElapsedTime > AttackSequenceDuration; }
	float GetMoveAllowTimeInterval() const { return MoveAllowTimeInterval; }
	void Attack()
	{
		bAttacking = true;
	}

	void Update(float DeltaTime)
	{
		if (IsAttacking())
		{
			ElapsedTime += DeltaTime;
		}
	}

	bool CanCombo() const
	{
		return ElapsedTime >= ComboCheckStart && ElapsedTime <= ComboCheckEnd;
	}

	void Reset()
	{
		ElapsedTime = 0.f;
		bAttacking = false;
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
	TArray<FVector> PrevAttackTraceFootstep;
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
	void CacheAttackTraceFootstep();

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
	virtual void BeginPlay() override;
	virtual void InternalUpdateWeapon(float DeltaTime) override;

private:
	void UpdateAttackSequence(float DeltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool CanAttack() const override;
	virtual void DoAttack() override;
	const FDSWeaponAttackSequence* GetAttackSequence(int32 Index) const;
	FVector GetSocketLocation(const FName& SocketName);

protected:
	virtual void InternalUnequipped() override;

private:
	FDSWeaponAttackSequence* GetAttackSequence(int32 Index);
	void DisableCharacterMovement();

	// {{ Weapon attack check
	void CheckAttackHit(float DeltaTime);
	// }} Weapon attack check

public:
	FTimerHandle MoveInputTimer;

	UPROPERTY(EditAnywhere)
	FDSAttackHitCheckHelper AttackHitCheckHelper;

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxCombo = 1;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDSWeaponAttackSequence> AttackSequence;

	UPROPERTY(Transient)
	int32 CurrentCombo = 0;


};
