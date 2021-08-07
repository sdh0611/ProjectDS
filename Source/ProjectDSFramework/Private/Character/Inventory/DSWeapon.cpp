// All rights reserve SDH (2021 ~ )


#include "DSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DSCharacterBase.h"

ADSWeapon::ADSWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bWeaponActive = false;

	AttachSocketNameOnActivated = TEXT("weapon_r");
	AttachSocketNameOnDeactivated = TEXT("weapon_unequipped");
}

void ADSWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ADSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void ADSWeapon::Equipped(ADSCharacterBase * EquipCharacter)
{
	Super::Equipped(EquipCharacter);

	WeaponActivated();
}

void ADSWeapon::Unequipped()
{
	Super::Unequipped();

	WeaponDeactivated();
}

void ADSWeapon::WeaponActivated()
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->GetMesh())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketNameOnActivated);
		if (EquipAnim)
		{
			OwnerCharacter->PlayAnimMontage(EquipAnim);
		}

		bWeaponActive = true;
	}
}

void ADSWeapon::WeaponDeactivated()
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->GetMesh())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketNameOnDeactivated);
		if (UnequipAnim)
		{
			OwnerCharacter->PlayAnimMontage(UnequipAnim);

		}

		bWeaponActive = false;
	}
}

void ADSWeapon::AttachWeapon()
{
}

void ADSWeapon::DetachWeapon()
{
}
