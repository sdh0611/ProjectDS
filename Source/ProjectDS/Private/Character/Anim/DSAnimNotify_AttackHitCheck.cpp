// All rights reserve SDH (2021 ~ )


#include "DSAnimNotify_AttackHitCheck.h"
#include "DSCharacterBase.h"
#include "DSWeaponSwordBase.h"

UDSAnimNotify_AttackHitCheck::UDSAnimNotify_AttackHitCheck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UDSAnimNotify_AttackHitCheck::BranchingPointNotify(FBranchingPointNotifyPayload & BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (BranchingPointPayload.SkelMeshComponent && BranchingPointPayload.SkelMeshComponent->GetOwner())
	{
		ADSCharacterBase* OwnerCharacter = Cast<ADSCharacterBase>(BranchingPointPayload.SkelMeshComponent->GetOwner());
		if (IsValid(OwnerCharacter))
		{
			ADSWeaponSwordBase* Weapon = OwnerCharacter->GetCurrentWeapon<ADSWeaponSwordBase>();
			if (Weapon)
			{
				if (bIsStartHitCheck)
				{
					Weapon->AttackHitCheckHelper.HitCheckStart();
				}
				else
				{
					Weapon->AttackHitCheckHelper.HitCheckEnd();
				}
			}
		}

	}

}

#if WITH_EDITOR
bool UDSAnimNotify_AttackHitCheck::CanBePlaced(UAnimSequenceBase * Animation) const
{
	return IsValid(Animation) && Animation->IsA(UAnimMontage::StaticClass());
}
#endif //WITH_EDITOR
