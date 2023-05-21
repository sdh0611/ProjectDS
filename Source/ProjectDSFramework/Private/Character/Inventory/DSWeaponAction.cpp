// SDH, All rights reserved. (2021 ~ )


#include "Character/Inventory/DSWeaponAction.h"
#include "Character/Inventory/DSWeapon.h"

UDSWeaponAction::UDSWeaponAction()
{
}

void UDSWeaponAction::SetOwnerWeapon(ADSWeapon* InOwnerWeapon)
{
	OwnerWeapon = InOwnerWeapon;
}
