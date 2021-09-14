// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DSPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	// ~ Begin ADSPlayerCameraManager Interface
	void SetTarget(APawn* NewTarget);
	void ReleaseTarget();
	void UpdateTargetState(float MaxDistance, const FVector& StartLocation);
	FORCEINLINE bool IsTargeting() { return TargetPawn.IsValid(); }
	// ~ End ADSPlayerCameraManager Interface

public:
	// ~ Begin APlayerCameraManager Interface
	/**
	 * Called to give PlayerCameraManager a chance to adjust view rotation updates before they are applied.
	 * e.g. The base implementation enforces view rotation limits using LimitViewPitch, et al.
	 * @param DeltaTime - Frame time in seconds.
	 * @param OutViewRotation - In/out. The view rotation to modify.
	 * @param OutDeltaRot - In/out. How much the rotation changed this frame.
	 */
	virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
	// ~ End APlayerCameraManager Interface

protected:
	// ~ Begin APlayerCameraManager Interface
	/**
	 * Calculates an updated POV for the given viewtarget.
	 * @param	OutVT		ViewTarget to update.
	 * @param	DeltaTime	Delta Time since last camera update (in seconds).
	 */
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	// ~ End APlayerCameraManager Interface	

private:
	TWeakObjectPtr<class APawn> TargetPawn;

protected:
	UPROPERTY(EditDefaultsOnly)
	float InterpSpeedToTarget = 15.f;

};
