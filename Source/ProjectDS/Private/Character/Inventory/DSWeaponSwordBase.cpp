// All rights reserve SDH (2021 ~ )


#include "DSWeaponSwordBase.h"
#include "DSCharacterBase.h"
#include "DrawDebugHelpers.h"


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
	if (CurSequence && CurSequence->IsAttacking())
	{
		if (CurSequence->IsSequenceEnd())
		{
			CurSequence->Reset();
			CurrentCombo = 0;
			if (OwnerCharacter.IsValid())
			{
				OwnerCharacter->EnableCharacterInput(ADSCharacterBase::EActiveInputFlag::InputAll);
			}
		}
		else
		{
			CurSequence->Update(DeltaTime);
		}
	}
}

void ADSWeaponSwordBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADSWeaponSwordBase::CanAttack() const
{
	const bool bCanAttack = Super::CanAttack() && IsWeaponArmed();

	return bCanAttack;
	//return CurrentCombo < AttackSequence.Num();
}

void ADSWeaponSwordBase::DoAttack()
{
	if (OwnerCharacter.IsValid())
	{
		FDSWeaponAttackSequence* Sequence = GetAttackSequence(CurrentCombo);
		if (Sequence)
		{
			bool bCanAttack = false;
			if (!Sequence->IsAttacking())
			{
				bCanAttack = true;
			}
			else if (Sequence->CanCombo() && AttackSequence.IsValidIndex(CurrentCombo+1))
			{
				Sequence->Reset();
				Sequence = GetAttackSequence(++CurrentCombo);
				if (Sequence)
				{
					bCanAttack = true;
				}
			}

			if (bCanAttack)
			{
				Sequence->Attack();
				DisableCharacterMovement();
				//OwnerCharacter->SetCharacterInputFlag(ADSCharacterBase::EActiveInputFlag::InputAll);
				//OwnerCharacter->DisableCharacterInput(ADSCharacterBase::EActiveInputFlag::InputJump | ADSCharacterBase::EActiveInputFlag::InputEquipWeapon);
				//GetWorldTimerManager().SetTimer(MoveInputTimer, this, &ADSWeaponSwordBase::DisableCharacterMovement, Sequence->GetMoveAllowTimeInterval(), false);
				if (Sequence->AttackAnim.WeaponAnim)
				{
					OwnerCharacter->PlayAnimMontage(Sequence->AttackAnim.WeaponAnim, Sequence->AttackAnim.PlayRate);
				}
			}
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

void ADSWeaponSwordBase::DisableCharacterMovement()
{
	if (OwnerCharacter.IsValid())
	{
		// Attack, LookUp, Turn 빼고 모든 입력을 막음.
		uint16 EnableInput = ADSCharacterBase::EActiveInputFlag::InputAttack | ADSCharacterBase::EActiveInputFlag::InputLookUp | ADSCharacterBase::EActiveInputFlag::InputTurn;
		OwnerCharacter->SetCharacterInputFlag(EnableInput);
	}
}
