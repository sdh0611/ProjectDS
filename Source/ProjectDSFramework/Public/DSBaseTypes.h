// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "DSBaseTypes.generated.h"

UENUM()
enum class EAttackInputType : uint8
{
	Attack = 0,
	AttackRelease,
	AltAttack,
	AltAttackRelease,
	ChargedAttack,
	WeaponSkill,
};

enum class EAttackType : uint8
{
	None = 0,
	Attack,
	ChargedAttack,
	WeaponSkill,
};

USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()
		
private:
	UPROPERTY()
	float Damage;

	UPROPERTY()
	uint8 DirtyByte;

public:
	FTakeHitInfo();

	void SetDirty();
	void SetDamage(float InDamage);

	float GetDamage() const { return Damage; }
};


