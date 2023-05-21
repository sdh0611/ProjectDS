// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerControllerBase.generated.h"

class UInputMappingContext;
class UDSInputSetting;

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
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	// ~ End APlayerController Interface

protected:
	// ~Begin ADSPlayerControllerBase Interface
	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);
	APawn* GetNearestTargetOnScreen();
	void SetTarget(APawn* NewTarget);
	void CheckTargetState();
	// ~End ADSPlayerControllerBase Interface

public:
	// ~Begin ADSPlayerControllerBase Interface
	bool IsTargeting() const;
	void LockOnTarget();
	void ReleaseTarget();
	void OnAttackHit();
	void ApplyInputMappingContext(UInputMappingContext* InMappingContext, int32 InPriority = 0);
	// ~End ADSPlayerControllerBase Interface

protected:
	// {{ Input binding delegates
	DECLARE_DELEGATE_OneParam(FControllerActionInputDelegate, bool);
	// }} Input binding delegates

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDSInputSetting> ControllerInputSetting;

private:
	UPROPERTY(Transient)
	TObjectPtr<class ADSPlayerCameraManager> DSPlayerCameraManager;

	UPROPERTY(EditAnywhere, Category = "LockOnTarget", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TargetSearchBoxHalfWidthRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "LockOnTarget", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TargetSearchBoxHalfHeightRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "LockOnTarget")
	float CheckTargetMaxDistance = 1500.f;

private:
	UPROPERTY(EditAnywhere, Category = "CameraShake", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> AttackHitCameraShakeClass;

};
