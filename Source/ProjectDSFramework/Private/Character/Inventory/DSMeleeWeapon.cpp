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

bool ADSMeleeWeapon::CanHandleAttackInput(EAttackInputType TryAttackType) const
{
	const bool bCanAttack = Super::CanHandleAttackInput(TryAttackType) && IsWeaponArmed();

	return bCanAttack;
}

void ADSMeleeWeapon::InternalEquipped()
{
	Super::InternalEquipped();

}

void ADSMeleeWeapon::HandleAttackInput(EAttackInputType InAttackInputType)
{
	Super::HandleAttackInput(InAttackInputType);

	EAttackType TryAttackType = EAttackType::None;

	switch (InAttackInputType)
	{
	case EAttackInputType::Attack:
	case EAttackInputType::ChargedAttack:
		TryAttack(InAttackInputType);
		break;
	case EAttackInputType::AltAttack:
		DoGuard();
		break;
	case EAttackInputType::WeaponSkill:
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

void ADSMeleeWeapon::TryAttack(EAttackInputType InAttackInput)
{
	// Determine try attack type
	EAttackType TryAttackType = EAttackType::None;

	switch (InAttackInput)
	{
	case EAttackInputType::Attack:
		TryAttackType = EAttackType::Attack;
		break;
	case EAttackInputType::ChargedAttack:
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

