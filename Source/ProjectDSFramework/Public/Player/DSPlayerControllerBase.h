// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerControllerBase();

protected:
	// ~ Begin APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;
	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
	// ~ End APlayerController Interface

public:
	// ~ Begin APlayerController Interface
	/** spawn cameras for servers and owning players */
	virtual void SpawnPlayerCameraManager() override;
	// ~ End APlayerController Interface

protected:
	// ~Begin ADSPlayerControllerBase Interface
	void LockOnTarget();
	APawn* GetTargetOnScreen();
	void SetTarget(APawn* NewTarget);
	void ReleaseTarget();
	// ~End ADSPlayerControllerBase Interface

public:
	// ~Begin ADSPlayerControllerBase Interface
	bool IsTargeting() const;
	// ~End ADSPlayerControllerBase Interface

protected:
	// {{ Input binding delegates
	DECLARE_DELEGATE_OneParam(FControllerActionInputDelegate, bool);
	// }} Input binding delegates

private:
	UPROPERTY(Transient)
	class ADSPlayerCameraManager* DSPlayerCameraManager;

public:
	static const uint16 RotationInputFlag;

};
