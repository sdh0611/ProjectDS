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
	void SetTarget(APawn* NewTarget);
	void ReleaseTarget();
	FORCEINLINE bool IsTargeting() { return TargetPawn.IsValid(); }

private:
	TWeakObjectPtr<class APawn> TargetPawn;

};
