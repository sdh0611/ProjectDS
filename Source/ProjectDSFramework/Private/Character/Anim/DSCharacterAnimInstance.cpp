// All rights reserve SDH (2021 ~ )


#include "DSCharacterAnimInstance.h"
#include "DSCharacterMovementComponent.h"
#include "DSCharacterBase.h"

UDSCharacterAnimInstance::UDSCharacterAnimInstance()
{
	bCrouching = false;
	bSprinting = false;
	bFalling = false;
	bAccel = false;
	CurrentSpeed2D = 0.f;
	RunAnimPlayRate = 1.f;
	SpeedThresholdToCalcRunAnimPlayRate = 300.f;
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
			CurrentSpeed2D = DSMovement->Velocity.Size2D();
			if (CurrentSpeed2D > SpeedThresholdToCalcRunAnimPlayRate)
			{
				CalcRunAnimPlayRate(CurrentSpeed2D, SpeedThresholdToCalcRunAnimPlayRate);
			}
			else
			{
				RunAnimPlayRate = 1.f;
			}
		}
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
