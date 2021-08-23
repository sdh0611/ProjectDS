// All rights reserve SDH (2021 ~ )


#include "DSWeaponSwordBase.h"
#include "DSCharacterBase.h"

ADSWeaponSwordBase::ADSWeaponSwordBase()
{
}

void ADSWeaponSwordBase::BeginPlay()
{
	Super::BeginPlay();

}

void ADSWeaponSwordBase::InternalUpdateWeapon(float DeltaTime)
{
	Super::InternalUpdateWeapon(DeltaTime);

	UpdateAttackSequence(DeltaTime);
}

void ADSWeaponSwordBase::UpdateAttackSequence(float DeltaTime)
{
	FDSWeaponAttackSequence* CurSequence = GetAttackSequence(CurrentCombo);
	if (CurSequence)
	{
		CurSequence->Update(DeltaTime);
	}
}

void ADSWeaponSwordBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADSWeaponSwordBase::CanAttack() const
{
	const FDSWeaponAttackSequence* Sequence = GetAttackSequence(CurrentCombo);
	if (Sequence)
	{
		return !Sequence->IsAttacking() || Sequence->CanCombo();
	}

	return false;
}

void ADSWeaponSwordBase::DoAttack()
{
	if (CanAttack())
	{
		FDSWeaponAttackSequence* Sequence = GetAttackSequence(CurrentCombo);
		if (Sequence)
		{

		}
	}
}

const FDSWeaponAttackSequence* ADSWeaponSwordBase::GetAttackSequence(int32 Index) const
{
	if (AttackSequence.IsValidIndex(Index))
	{
		return &AttackSequence[Index];
	}

	return nullptr;
}

FDSWeaponAttackSequence* ADSWeaponSwordBase::GetAttackSequence(int32 Index)
{
	if (AttackSequence.IsValidIndex(Index))
	{
		return &AttackSequence[Index];
	}

	return nullptr;
}
