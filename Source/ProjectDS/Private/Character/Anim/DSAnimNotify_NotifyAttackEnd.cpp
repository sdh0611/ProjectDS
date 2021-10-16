// SDH, All rights reserved. (2021 ~ )


#include "DSAnimNotify_NotifyAttackEnd.h"
#include "DSCharacterBase.h"
#include "DSWeapon.h"


UDSAnimNotify_NotifyAttackEnd::UDSAnimNotify_NotifyAttackEnd(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDSAnimNotify_NotifyAttackEnd::BranchingPointNotify(FBranchingPointNotifyPayload & BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (BranchingPointPayload.SkelMeshComponent && BranchingPointPayload.SkelMeshComponent->GetOwner())
	{
		ADSCharacterBase* DSCharacter = Cast<ADSCharacterBase>(BranchingPointPayload.SkelMeshComponent->GetOwner());
		if (IsValid(DSCharacter))
		{
			ADSWeapon* Weapon = DSCharacter->GetCurrentWeapon<ADSWeapon>();
			if (IsValid(Weapon))
			{
				// TODO : 해당 노티파이를 쓰지 않을 수 있도록 AnimMontage의 FAnimMontageInstance.OnMontageEnded 딜리게이트를 활용할 방안을 찾아보자.
				//Weapon->OnAttackEnd();
			}
		}
	}

}

#if WITH_EDITOR
bool UDSAnimNotify_NotifyAttackEnd::CanBePlaced(UAnimSequenceBase * Animation) const
{
	return Animation && Animation->IsA(UAnimMontage::StaticClass());
}
#endif // WITH_EDITOR
