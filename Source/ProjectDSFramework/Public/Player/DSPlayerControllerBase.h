// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;


};
