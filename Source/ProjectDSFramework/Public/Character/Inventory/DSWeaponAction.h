// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSWeaponAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTDSFRAMEWORK_API UDSWeaponAction : public UObject
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditAnywhere)
	float TestProperty;

};
