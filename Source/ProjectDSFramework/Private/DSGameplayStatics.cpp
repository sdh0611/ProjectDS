// SDH, All rights reserved. (2021 ~ )


#include "DSGameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

bool UDSGameplayStatics::WasActorRecentlyRendered(AActor * CheckActor, float Tolerance)
{
	const float RealTolerance = FMath::Min<float>(0.2f, Tolerance);
	bool bRecentlyRendered = false;

	if (IsValid(CheckActor))
	{
		// Check skeletal mesh was recently rendered.
		TArray<USkeletalMeshComponent*> SkelMeshes;
		CheckActor->GetComponents(SkelMeshes, true);
		if (SkelMeshes.Num() > 0)
		{
			for (auto SkelMesh : SkelMeshes)
			{
				if (SkelMesh->WasRecentlyRendered(RealTolerance))
				{
					bRecentlyRendered = true;
					break;
				}
			}
		}
		else
		{
			// If the actor dosen't owned skeleltal mesh component, check rendered or not via using actor's function.
			bRecentlyRendered = CheckActor->WasRecentlyRendered(RealTolerance);
		}
	}

	return bRecentlyRendered;
}
