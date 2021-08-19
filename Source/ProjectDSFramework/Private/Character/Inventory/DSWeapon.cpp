// All rights reserve SDH (2021 ~ )


#include "DSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DSCharacterBase.h"
#include "Net/UnrealNetwork.h"

ADSWeapon::ADSWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bWeaponArmed = false;

	AttachSocketName = TEXT("weapon_r");
	AttachSocketNameOnDeactivated = TEXT("weapon_unequipped");
}

void ADSWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ADSWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ADSWeapon, bWeaponArmed);
}

void ADSWeapon::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	//DOREPLIFETIME_ACTIVE_OVERRIDE(ADSWeapon, bWeaponArmed, IsEquipped());
}

void ADSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEquipped())
	{
		InternalUpdateWeapon(DeltaTime);
	}
}

void ADSWeapon::InternalUpdateWeapon(float DeltaTime)
{

}

void ADSWeapon::InternalEquipped()
{
	Super::InternalEquipped();

	SetWeaponArmed(true);
}

void ADSWeapon::InternalUnequipped()
{
	Super::InternalUnequipped();

	SetWeaponArmed(false);

}

void ADSWeapon::WeaponArmed()
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->GetMesh())
	{
		if (EquipAnim)
		{
			OwnerCharacter->PlayAnimMontage(EquipAnim);
		}

	}
}

void ADSWeapon::WeaponUnarmed(bool bPlayAnim)
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->GetMesh())
	{
		if (bPlayAnim && UnequipAnim)
		{
			OwnerCharacter->PlayAnimMontage(UnequipAnim);
		}
	}
}

void ADSWeapon::SetWeaponArmed(bool bIsArmed)
{
	bWeaponArmed = bIsArmed;
	if (IsNetMode(NM_Standalone) || IsNetMode(NM_ListenServer))
	{
		OnRep_WeaponArmed();
	}
}

void ADSWeapon::OnRep_WeaponArmed()
{
	if (IsEquipped())
	{
		if (bWeaponArmed)
		{
			WeaponArmed();
		}
		else
		{
			WeaponUnarmed();
		}
	}
}

