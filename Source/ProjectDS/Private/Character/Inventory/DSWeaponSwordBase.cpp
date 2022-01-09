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

bool ADSWeaponSwordBase::CanAttack(EAttackInputType TryAttackType) const
{
	const bool bCanAttack = Super::CanAttack(TryAttackType) && IsWeaponArmed();

	return bCanAttack;
}

void ADSWeaponSwordBase::InternalEquipped()
{
	Super::InternalEquipped();

}

void ADSWeaponSwordBase::TryAttack(EAttackInputType TryAttackType)
{
	Super::TryAttack(TryAttackType);
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
