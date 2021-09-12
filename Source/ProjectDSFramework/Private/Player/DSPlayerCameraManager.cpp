// SDH, All rights reserved. (2021 ~ )


#include "DSPlayerCameraManager.h"
#include "DSCharacterBase.h"
#include "DSPlayerControllerBase.h"

void ADSPlayerCameraManager::SetTarget(APawn * NewTarget)
{
	if (IsValid(NewTarget))
	{
		TargetPawn = NewTarget;
	}
}

void ADSPlayerCameraManager::ReleaseTarget()
{
	TargetPawn.Reset();
}
