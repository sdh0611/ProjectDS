// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "DSWeapon.h"
#include "DSWeaponSwordBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDS_API ADSWeaponSwordBase : public ADSWeapon
{
	GENERATED_BODY()
	
public:
	ADSWeaponSwordBase();

protected:
	virtual void BeginPlay() override;
	virtual void InternalUpdateWeapon(float DeltaTime) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool CanAttack() const override;
	virtual void DoAttack() override;


};
