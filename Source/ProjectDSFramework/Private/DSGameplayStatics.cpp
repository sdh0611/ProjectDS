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
		TArray<USkeletalMeshComponent*> SkelMeshes;
		CheckActor->GetComponents(SkelMeshes, true);
		for (auto SkelMesh : SkelMeshes)
		{
			if (SkelMesh->WasRecentlyRendered(RealTolerance))
			{
				bRecentlyRendered = true;
				break;
			}
		}
	}

	return bRecentlyRendered;
}
