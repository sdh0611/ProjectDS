// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSInputSetting.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT()
struct FDSInputMappingContextSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	int32 Priority;

	FDSInputMappingContextSet()
		: InputMappingContext(nullptr)
		, Priority(INDEX_NONE)
	{
	}

	bool IsValidSet() const
	{
		return InputMappingContext != nullptr && Priority > INDEX_NONE;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTDSFRAMEWORK_API UDSInputSetting : public UDataAsset
{
	GENERATED_BODY()

public:
	UInputAction* GetInputAction(FName InInputActionName) const;
	FDSInputMappingContextSet& GetInputMappingContextSet() { return InputMappingContextSet; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UInputAction*> InputActionTable;

	UPROPERTY(EditDefaultsOnly)
	FDSInputMappingContextSet InputMappingContextSet;
};
