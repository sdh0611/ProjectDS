// SDH, All rights reserved. (2021 ~ )


#include "DSAnimNotify_AttachWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DSCharacterBase.h"
#include "DSWeapon.h"

UDSAnimNotify_AttachWeapon::UDSAnimNotify_AttachWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
	AttachSocketName = NAME_None;
}

void UDSAnimNotify_AttachWeapon::BranchingPointNotify(FBranchingPointNotifyPayload & BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (BranchingPointPayload.SkelMeshComponent)
	{
		ADSCharacterBase* OwnerCharacter = Cast<ADSCharacterBase>(BranchingPointPayload.SkelMeshComponent->GetOwner());
		if (IsValid(OwnerCharacter))
		{
			ADSWeapon* Weapon = OwnerCharacter->GetCurrentWeapon();
			if (IsValid(Weapon) && Weapon->IsEquipped())
			{
				Weapon->AttachToCharacter(AttachSocketName);
			}
		}
	}

}

bool UDSAnimNotify_AttachWeapon::CanBePlaced(UAnimSequenceBase * Animation) const
{
	return Animation && Animation->IsA(UAnimMontage::StaticClass());
}
