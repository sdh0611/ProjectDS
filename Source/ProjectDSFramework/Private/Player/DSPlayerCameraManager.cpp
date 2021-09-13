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

void ADSPlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator & OutViewRotation, FRotator & OutDeltaRot)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);

	// Interpolate POV rotation
	const APawn* const ViewPawn = ViewTarget.GetTargetPawn();
	if (IsTargeting() && IsValid(ViewPawn))
	{
		const FVector ToTarget = TargetPawn->GetActorLocation() - ViewPawn->GetActorLocation();
		const FRotator ToTargetRot = ToTarget.Rotation();

		OutViewRotation = FMath::RInterpTo(OutViewRotation, ToTargetRot, DeltaTime, InterpSpeedToTarget);
	}
}

void ADSPlayerCameraManager::UpdateViewTarget(FTViewTarget & OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);


}
