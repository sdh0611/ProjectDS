// SDH, All rights reserved. (2021 ~ )


#include "DSAnimNotify_ComboCheck.h"
#include "DSCharacterBase.h"
#include "DSWeaponSwordBase.h"

UDSAnimNotify_ComboCheck::UDSAnimNotify_ComboCheck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UDSAnimNotify_ComboCheck::BranchingPointNotify(FBranchingPointNotifyPayload & BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (BranchingPointPayload.SkelMeshComponent && BranchingPointPayload.SkelMeshComponent->GetOwner())
	{
		ADSCharacterBase* OwnerCharacter = Cast<ADSCharacterBase>(BranchingPointPayload.SkelMeshComponent->GetOwner());
		if (IsValid(OwnerCharacter))
		{
			ADSWeaponSwordBase * Weapon = OwnerCharacter->GetCurrentWeapon<ADSWeaponSwordBase>();
			if (Weapon)
			{
				if (bIsStartComboCheck)
				{
					Weapon->RequestComboCheckStart();
				}
				else
				{
					Weapon->RequestComboCheckEnd();
				}
			}
		}

	}

}

#if WITH_EDITOR
bool UDSAnimNotify_ComboCheck::CanBePlaced(UAnimSequenceBase * Animation) const
{
	return IsValid(Animation) && Animation->IsA(UAnimMontage::StaticClass());
}
#endif //WITH_EDITOR