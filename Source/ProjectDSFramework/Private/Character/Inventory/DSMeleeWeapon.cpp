// SDH, All rights reserved. (2021 ~ )


#include "Character/Inventory/DSMeleeWeapon.h"
#include "DSCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "DSCharacterAnimInstance.h"


ADSMeleeWeapon::ADSMeleeWeapon()
{
}

void ADSMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ADSMeleeWeapon::InternalUpdateWeapon(float DeltaTime)
{
	Super::InternalUpdateWeapon(DeltaTime);

}

bool ADSMeleeWeapon::CanHandleWeaponActionInput(EWeaponActionInput TryAttackType) const
{
	const bool bCanAttack = Super::CanHandleWeaponActionInput(TryAttackType) && IsWeaponArmed();

	return bCanAttack;
}

void ADSMeleeWeapon::InternalEquipped()
{
	Super::InternalEquipped();

}

void ADSMeleeWeapon::HandleWeaponActionInput(EWeaponActionInput InWeaponActionInput)
{
	Super::HandleWeaponActionInput(InWeaponActionInput);

	EAttackType TryAttackType = EAttackType::None;

	switch (InWeaponActionInput)
	{
	case EWeaponActionInput::Attack:
	case EWeaponActionInput::ChargedAttack:
		TryAttack(InWeaponActionInput);
		break;
	case EWeaponActionInput::AltAttack:
		DoGuard();
		break;
	case EWeaponActionInput::WeaponSkill:
		DoWeaponSkill();
		break;
	}

}

void ADSMeleeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ADSMeleeWeapon::PostNetReceive()
{
	Super::PostNetReceive();

}

void ADSMeleeWeapon::InternalUnequipped()
{
	Super::InternalUnequipped();

}

void ADSMeleeWeapon::TryAttack(EWeaponActionInput InAttackInput)
{
	// Determine try attack type
	EAttackType TryAttackType = EAttackType::None;

	switch (InAttackInput)
	{
	case EWeaponActionInput::Attack:
		TryAttackType = EAttackType::Attack;
		break;
	case EWeaponActionInput::ChargedAttack:
		TryAttackType = EAttackType::ChargedAttack;
		break;
	default:
		break;
	}

	// Set pending attack data
	PendingAttack.AttackType = TryAttackType;

}

void ADSMeleeWeapon::DoAttack()
{



}

void ADSMeleeWeapon::DoGuard()
{

}

void ADSMeleeWeapon::DoWeaponSkill()
{

}

bool ADSMeleeWeapon::IsAttacking() const
{
	return true;
}

