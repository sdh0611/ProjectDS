// SDH, All rights reserved. (2021 ~ )


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

	if (WeaponActions.Num() > 0)
	{
		for (FDSWeaponActionData& InWeaponAction : WeaponActions)
		{
			if (InWeaponAction.WeaponAction)
			{
				InWeaponAction.WeaponAction->SetOwnerWeapon(this);
			}
		}
	}
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

void ADSWeapon::PostNetReceive()
{
	Super::PostNetReceive();

}

void ADSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEquipped())
	{
		InternalUpdateWeapon(DeltaTime);
	}

	if (WeaponActions.Num() > 0)
	{
		for (FDSWeaponActionData& InWeaponAction : WeaponActions)
		{
			if (InWeaponAction.WeaponAction && InWeaponAction.WeaponAction->ShouldTickAction())
			{
				InWeaponAction.WeaponAction->TickAction(DeltaTime);
			}
		}

	}

}
void ADSWeapon::HandleWeaponActionInput(EWeaponActionInput InWeaponActionInput)
{
	UDSWeaponAction* WeaponAction = GetWeaponAction(InWeaponActionInput);
	if (WeaponAction)
	{
		WeaponAction->DoAction();
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
	if (IsValid(OwnerCharacter) && OwnerCharacter->GetMesh())
	{
		if (EquipAnim)
		{
			OwnerCharacter->PlayAnimMontage(EquipAnim);
		}
	}
}

void ADSWeapon::WeaponUnarmed(bool bPlayAnim)
{
	if (IsValid(OwnerCharacter) && OwnerCharacter->GetMesh())
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

UDSWeaponAction* ADSWeapon::GetWeaponAction(EWeaponActionInput InInputType) const
{
	for (const FDSWeaponActionData& WeaponActionData : WeaponActions)
	{
		if (WeaponActionData.WeaponActionInput == InInputType)
		{
			return WeaponActionData.WeaponAction;
		}
	}

	return nullptr;
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

