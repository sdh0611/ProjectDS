// SDH, All rights reserved. (2021 ~ )


#include "DSCheatManager.h"
#include "DSCharacterBase.h"
#include "DSPlayerControllerBase.h"
#include "DSWeapon.h"

void UDSCheatManager::WeaponUnequipTest()
{
	APlayerController* OwnerPC = GetOuterADSPlayerControllerBase();
	if (OwnerPC)
	{
		ADSCharacterBase* DSCharacter = Cast<ADSCharacterBase>(OwnerPC->GetPawn());
		if (DSCharacter && DSCharacter->GetCurrentWeapon())
		{
			DSCharacter->UnequipWeapon();
		}
	}

}
