// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "DSGameplayStatics.generated.h"

class IEnhancedInputSubsystemInterface;

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:
	static bool WasActorRecentlyRendered(AActor* CheckActor, float Tolerance = 0.2f);
	static IEnhancedInputSubsystemInterface* GetEnhancedInputSubsystem(APlayerController* InPlayerController);
};
