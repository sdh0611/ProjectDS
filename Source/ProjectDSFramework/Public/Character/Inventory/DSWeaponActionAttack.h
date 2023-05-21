// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSWeaponAction.h"
#include "DSWeaponActionAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSWeaponActionAttack : public UDSWeaponAction
{
	GENERATED_BODY()
	
public:
	UDSWeaponActionAttack();

public:
	virtual void TickAction(float DeltaTime) override;
	virtual void DoAction() override;
	virtual void StopAction() override;
	virtual bool ShouldTickAction() override;

private:
	void InternalHitCheck(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere)
	FName HitCheckStartSocketName;

	UPROPERTY(EditAnywhere)
	FName HitCheckEndtSocketName;

	UPROPERTY(EditAnywhere)
	float CheckInterval;

	UPROPERTY(EditAnywhere)
	FDSWeaponAnimData AttackAnim;

	UPROPERTY(EditAnywhere, Meta=(ClampMin=0.0))
	float DamageMultiplier;

private:
	bool bNeedHitCheck;
	float ElapsedTickTime;

};
