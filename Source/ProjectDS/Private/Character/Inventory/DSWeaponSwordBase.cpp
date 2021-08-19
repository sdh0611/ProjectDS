// All rights reserve SDH (2021 ~ )


#include "DSWeaponSwordBase.h"
#include "DSCharacter.h"

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

void ADSWeaponSwordBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADSWeaponSwordBase::CanAttack() const
{
	return false;
}

void ADSWeaponSwordBase::DoAttack()
{
}
