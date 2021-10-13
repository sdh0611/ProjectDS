// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DSAnimNotify_ComboCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDS_API UDSAnimNotify_ComboCheck : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

public:
	UPROPERTY(EditAnywhere)
	uint8 bIsStartComboCheck : 1;
	
};
