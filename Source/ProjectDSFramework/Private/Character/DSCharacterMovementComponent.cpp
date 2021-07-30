// All rights reserve SDH (2021 ~ )


#include "DSCharacterMovementComponent.h"
#include "DSCharacterBase.h"

UDSCharacterMovementComponent::UDSCharacterMovementComponent()
{
	SprintSpeedModifier = 1.5f;
	bSprinting = false;
}

void UDSCharacterMovementComponent::SetUpdatedComponent(USceneComponent * NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	if (IsValid(CharacterOwner))
	{
		ADSCharacterBase* NewDSCharacter = Cast<ADSCharacterBase>(CharacterOwner);
		if (NewDSCharacter)
		{
			OwnerDSCharacter = NewDSCharacter;
		}
	}

}

float UDSCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (bSprinting)
	{
		MaxSpeed *= SprintSpeedModifier;
	}

	return MaxSpeed;
}

void UDSCharacterMovementComponent::SetSprinting(bool bSprint)
{
	bSprinting = bSprint;
}
