// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DSCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDSCharacterAnimInstance();

public:
	// ~ Begin UAnimInstance Interface
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// ~ End UAnimInstance Interface

protected:
	// ~ Begin UDSCharacterAnimInstance Interface
	void CalcRunAnimPlayRate(const float CurrentSpeed, const float SpeedThreshold);
	void UpdateMoveDirection(float DeltaSeconds, const FRotator& ActorRotation, const FVector& Velocity);
	void UpdateLeftHandIK(class ADSCharacterBase* OwnerCharacter);
	// ~ End UDSCharacterAnimInstance Interface

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bCrouching : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bFalling : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bAccel : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bArmed : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bTargeting : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bDied : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentSpeed2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RunAnimPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpeedThresholdToCalcRunAnimPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MoveDirection;

	UPROPERTY(EditDefaultsOnly, Category = HandIK)
	FName WeaponLeftHandIKSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = HandIK)
	FVector LeftHandIKBoneAdditiveOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LeftHandIKEffectorLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LeftHandIKAlpha;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll")
	FPoseSnapshot DeadPoseSnapshot;


};
