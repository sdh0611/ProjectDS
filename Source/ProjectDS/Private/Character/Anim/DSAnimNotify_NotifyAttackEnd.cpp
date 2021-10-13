// SDH, All rights reserved. (2021 ~ )


#include "DSAnimNotify_NotifyAttackEnd.h"
#include "DSCharacterBase.h"
#include "DSWeaponSwordBase.h"


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
			ADSWeaponSwordBase* Weapon = DSCharacter->GetCurrentWeapon<ADSWeaponSwordBase>();
			if (IsValid(Weapon))
			{
				// TODO : �ش� ��Ƽ���̸� ���� ���� �� �ֵ��� AnimMontage�� FAnimMontageInstance.OnMontageEnded ��������Ʈ�� Ȱ���� ����� ã�ƺ���.
				//Weapon->AttackEnd();
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
