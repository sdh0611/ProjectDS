// SDH, All rights reserved. (2021 ~ )


#include "DSWeaponSwordBase.h"
#include "DSCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "DSCharacterAnimInstance.h"


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

}

bool ADSWeaponSwordBase::CanHandleAttackInput(EAttackInputType TryAttackType) const
{
	const bool bCanAttack = Super::CanHandleAttackInput(TryAttackType) && IsWeaponArmed();

	return bCanAttack;
}

void ADSWeaponSwordBase::InternalEquipped()
{
	Super::InternalEquipped();

}

void ADSWeaponSwordBase::HandleAttackInput(EAttackInputType InAttackInputType)
{
	Super::HandleAttackInput(InAttackInputType);

	EAttackType TryAttackType = EAttackType::None;

	switch (InAttackInputType)
	{
	case EAttackInputType::Attack:
		DoAttack();
		break;
	case EAttackInputType::AltAttack:
		DoGuard();
		break;
	case EAttackInputType::WeaponSkill:
		DoWeaponSkill();
		break;
	}

}

void ADSWeaponSwordBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ADSWeaponSwordBase::PostNetReceive()
{
	Super::PostNetReceive();

}

void ADSWeaponSwordBase::InternalUnequipped()
{
	Super::InternalUnequipped();

}

void ADSWeaponSwordBase::DoAttack()
{
}

void ADSWeaponSwordBase::DoGuard()
{

}

void ADSWeaponSwordBase::DoWeaponSkill()
{

}