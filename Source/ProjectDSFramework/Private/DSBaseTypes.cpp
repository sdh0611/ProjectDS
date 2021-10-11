// SDH, All rights reserved. (2021 ~ )


#include "DSBaseTypes.h"

FTakeHitInfo::FTakeHitInfo()
	: Damage(0.f)
	, DirtyByte(0)
{
}

void FTakeHitInfo::SetDirty()
{
	DirtyByte = (++DirtyByte) % 255;
}

void FTakeHitInfo::SetDamage(float InDamage)
{
	Damage = FMath::Max<float>(0, InDamage);
	SetDirty();
}
