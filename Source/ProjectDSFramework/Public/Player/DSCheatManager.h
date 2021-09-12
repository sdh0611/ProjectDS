// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DSCheatManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Within = DSPlayerControllerBase)
class PROJECTDSFRAMEWORK_API UDSCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void WeaponUnequipTest();

};
