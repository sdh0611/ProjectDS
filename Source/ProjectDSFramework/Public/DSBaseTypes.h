// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "DSBaseTypes.generated.h"

USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()
		
private:
	UPROPERTY()
	float Damage;

	UPROPERTY()
	uint8 DirtyByte;

public:
	FTakeHitInfo();

	void SetDirty();
	void SetDamage(float InDamage);

	float GetDamage() const { return Damage; }
};


