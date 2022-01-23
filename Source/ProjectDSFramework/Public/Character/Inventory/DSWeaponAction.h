// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSWeaponAction.generated.h"

class ADSWeapon;

/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class PROJECTDSFRAMEWORK_API UDSWeaponAction : public UObject
{
	GENERATED_BODY()
	

public:
	UDSWeaponAction();

public:
	void SetOwnerWeapon(ADSWeapon* InOwnerWeapon);
	virtual void TickAction(float DeltaTime) { }
	virtual void DoAction() PURE_VIRTUAL(DoAction)
	virtual void StopAction() PURE_VIRTUAL(StopAction)
	virtual bool ShouldTickAction() { return false; }

protected:
	TWeakObjectPtr<ADSWeapon> OwnerWeapon;

};
