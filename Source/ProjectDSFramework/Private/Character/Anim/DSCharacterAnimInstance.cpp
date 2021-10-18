// SDH, All rights reserved. (2021 ~ )


#include "DSCharacterAnimInstance.h"
#include "DSCharacterMovementComponent.h"
#include "DSCharacterBase.h"
#include "..\..\..\Public\Character\Anim\DSCharacterAnimInstance.h"

UDSCharacterAnimInstance::UDSCharacterAnimInstance()
{
	bCrouching = false;
	bFalling = false;
	bAccel = false;
	bArmed = false;
	bTargeting = false;

	CurrentSpeed2D = 0.f;
	RunAnimPlayRate = 1.f;
	SpeedThresholdToCalcRunAnimPlayRate = 300.f;

	WeaponLeftHandIKSocketName = TEXT("ik_lefthand");
	LeftHandIKBoneAdditiveOffset = FVector::ZeroVector;
	LeftHandIKAlpha = 0.f;
}

void UDSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UDSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADSCharacterBase* DSCharacter = Cast<ADSCharacterBase>(TryGetPawnOwner());
	if (DSCharacter)
	{
		UDSCharacterMovementComponent* DSMovement = DSCharacter->GetDSCharacterMovement();
		if (DSMovement)
		{
			bCrouching = DSMovement->IsCrouching();
			bFalling = DSMovement->IsFalling();
			bAccel = (DSMovement->GetCurrentAcceleration().SizeSquared2D() > 0.f);
			bArmed = DSCharacter->IsArmed();
			bTargeting = DSCharacter->IsTargeting();

			CurrentSpeed2D = DSMovement->Velocity.Size2D();
			if (CurrentSpeed2D > SpeedThresholdToCalcRunAnimPlayRate)
			{
				CalcRunAnimPlayRate(CurrentSpeed2D, SpeedThresholdToCalcRunAnimPlayRate);
			}
			else
			{
				RunAnimPlayRate = 1.f;
			}

			// Property MoveDirection used when targeting state
			if (bTargeting)
			{
				UpdateMoveDirection(DeltaSeconds, DSCharacter->GetActorRotation(), DSMovement->Velocity);
			}
		}
	
		// Get left hand IK location
		UpdateLeftHandIK(DSCharacter);
	}
}

void UDSCharacterAnimInstance::CalcRunAnimPlayRate(const float CurrentSpeed, const float SpeedThreshold)
{
	const float SpeedDelta = CurrentSpeed - SpeedThreshold;
	if (SpeedDelta > 0.f)
	{
		RunAnimPlayRate = 1.f + (SpeedDelta / CurrentSpeed);
	}

}

void UDSCharacterAnimInstance::UpdateMoveDirection(float DeltaSeconds, const FRotator& ActorRotation, const FVector& Velocity)
{
	if (!Velocity.IsNearlyZero(10.f))
	{
		// Get actor's normalized velocity
		const FVector NormalizedVelocity = Velocity.GetSafeNormal2D();

		// Get actor's normalized forward vector
		const FRotator YawRot(0.f, ActorRotation.Yaw, 0.f);
		const FVector ActorFoward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);

		// Do cross project
		const FVector Cross = FVector::CrossProduct(NormalizedVelocity, ActorFoward);
		const bool bInverted = Cross.Z >= 0.f;

		// Do dot product
		const float Dot = FVector::DotProduct(ActorFoward, NormalizedVelocity);
		const float Radians = FMath::Acos(Dot);

		// Convert radians to degrees
		MoveDirection = FMath::RadiansToDegrees<float>(Radians);
		if (bInverted)
		{
			MoveDirection *= -1.f;
		}
	}
	else
	{
		MoveDirection = 0.f;
	}
}

void UDSCharacterAnimInstance::UpdateLeftHandIK(ADSCharacterBase * OwnerCharacter)
{
	if (IsValid(OwnerCharacter))
	{
		LeftHandIKAlpha = bArmed ? 1.f : 0.f;
		LeftHandIKEffectorLocation = OwnerCharacter->GetWeaponSocketLocation(WeaponLeftHandIKSocketName);
		//LeftHandIKEffectorLocation = OwnerCharacter->GetWeaponSocketComponentLocation(WeaponLeftHandIKSocketName);
		//LeftHandIKEffectorLocation += LeftHandIKBoneAdditiveOffset;
	}
}
