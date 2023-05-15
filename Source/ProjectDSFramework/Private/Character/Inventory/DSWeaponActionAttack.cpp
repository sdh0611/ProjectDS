// SDH, All rights reserved. (2021 ~ )


#include "Character/Inventory/DSWeaponActionAttack.h"
#include "Character/Inventory/DSWeapon.h"

UDSWeaponActionAttack::UDSWeaponActionAttack()
{
    DamageMultiplier = 1.f;
    bNeedHitCheck = false;
    ElapsedTickTime = 0.f;
}

void UDSWeaponActionAttack::TickAction(float DeltaTime)
{
    if (ElapsedTickTime > CheckInterval)
    {
        InternalHitCheck(DeltaTime);
    }
    else
    {
        ElapsedTickTime += DeltaTime;
    }
}

void UDSWeaponActionAttack::DoAction()
{
    bNeedHitCheck = true;
}

void UDSWeaponActionAttack::StopAction()
{
    bNeedHitCheck = false;
    ElapsedTickTime = 0.f;
}

bool UDSWeaponActionAttack::ShouldTickAction()
{
    return bNeedHitCheck;
}

void UDSWeaponActionAttack::InternalHitCheck(float DeltaTime)
{
}
