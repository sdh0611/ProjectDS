// SDH, All rights reserved. (2021 ~ )


#include "DSPlayerCameraManager.h"
#include "DSPlayerControllerBase.h"
#include "DSGameplayStatics.h"

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

void ADSPlayerCameraManager::CheckTargetState(float MaxDistance, const FVector& StartLocation)
{
	if (TargetPawn.IsValid())
	{
		const float MaxDistanceSquared = MaxDistance * MaxDistance;
		const float DistanceToTargetSquared = FVector::DistSquared(TargetPawn->GetActorLocation(), StartLocation);
		// Request release target
		if (DistanceToTargetSquared > MaxDistanceSquared || !UDSGameplayStatics::WasActorRecentlyRendered(TargetPawn.Get()))
		{
			ADSPlayerControllerBase* DSPC = Cast<ADSPlayerControllerBase>(PCOwner);
			if (IsValid(DSPC))
			{
				DSPC->ReleaseTarget();
			}
		}
	}
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
