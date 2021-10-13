// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DSAnimNotify_NotifyAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDS_API UDSAnimNotify_NotifyAttackEnd : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

};
