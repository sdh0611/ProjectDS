// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDSCharacterMovementComponent();

public:
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	// Instead of GetMaxSpeedModifier(Deprecated from 4.3)
	virtual float GetMaxSpeed() const override;
	void SetSprinting(bool bSprint);

private:
	UPROPERTY(EditAnywhere, Category = Move)
	float SprintSpeedModifier;
	
	uint8 bSprinting : 1;

	TWeakObjectPtr<class ADSCharacterBase> OwnerDSCharacter;
	
};
