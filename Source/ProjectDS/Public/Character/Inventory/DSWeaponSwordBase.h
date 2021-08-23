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

private:
	UPROPERTY(EditAnywhere)
	FDSWeaponAnim AttackAnim;

	UPROPERTY(EditAnywhere)
	float AttackSequenceDuration = 1.f;

	UPROPERTY(EditAnywhere)
	float ComboCheckStart = 0.f;

	UPROPERTY(EditAnywhere)
	float ComboCheckEnd = 0.f;

	float AttackSequenceStartTime = 0.f;

	float ElapsedTime = 0.f;

	uint8 bAttacking : 1;

public:
	FDSWeaponAttackSequence()
		: AttackAnim()
		, AttackSequenceDuration(1.f)
		, ComboCheckStart(0.f)
		, ComboCheckEnd(0.f)
		, AttackSequenceStartTime(0.f)
		, ElapsedTime(0.f)
		, bAttacking(false)
	{
	}

	void Attack()
	{
		bAttacking = true;
	}

	void Update(float DeltaTime)
	{
		if (bAttacking)
		{
			ElapsedTime += DeltaTime;
		}
	}

	bool IsAttacking() const { return bAttacking; }

	bool CanCombo() const
	{
		return ElapsedTime >= ComboCheckStart && ElapsedTime <= ComboCheckEnd;
	}

	void Reset()
	{
		bAttacking = false;
		AttackSequenceStartTime = 0.f;
	}
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

private:
	FDSWeaponAttackSequence* GetAttackSequence(int32 Index);
	

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxCombo = 1;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDSWeaponAttackSequence> AttackSequence;

	int32 CurrentCombo = 0;


};
