// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DSAnimNotify_AttachWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSAnimNotify_AttachWeapon : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

public:
	UPROPERTY(EditAnywhere)
	FName AttachSocketName;
};
