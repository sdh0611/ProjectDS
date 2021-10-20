// SDH, All rights reserved. (2021 ~ )


#include "DSPlayerControllerBase.h"
#include "Components/InputComponent.h"
#include "DSGameplayStatics.h"
#include "DSPlayerCameraManager.h"
#include "DSCharacterBase.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/HUD.h"
//#include "DSHUD.h"


ADSPlayerControllerBase::ADSPlayerControllerBase()
{
	PlayerCameraManagerClass = ADSPlayerCameraManager::StaticClass();

}

void ADSPlayerControllerBase::OnPossess(APawn * aPawn)
{
	Super::OnPossess(aPawn);

}

void ADSPlayerControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

}

void ADSPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void ADSPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent && InputComponent->IsRegistered())
	{
		InputComponent->BindAction(TEXT("LockOnTarget"), EInputEvent::IE_Pressed, this, &ADSPlayerControllerBase::LockOnTarget);
	}
}

void ADSPlayerControllerBase::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	if (PlayerCameraManager)
	{
		ADSPlayerCameraManager* SpawnedCameraManager = Cast<ADSPlayerCameraManager>(PlayerCameraManager);
		if (IsValid(SpawnedCameraManager))
		{
			DSPlayerCameraManager = SpawnedCameraManager;
		}
	}

}

void ADSPlayerControllerBase::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction & ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (IsTargeting())
	{
		CheckTargetState();
	}
}

APawn * ADSPlayerControllerBase::GetNearestTargetOnScreen()
{
	int32 ViewportWidth, ViewportHeight;

	GetViewportSize(ViewportWidth, ViewportHeight);
	if (ViewportWidth == 0 || ViewportHeight == 0)
	{
		// Occur undefined behavior 
		return nullptr;
	}

	// Make target search box on screen
	const FVector2D BoxCenter((float)ViewportWidth / 2.f, (float)ViewportHeight / 2.f);
	const FVector2D BoxMin(BoxCenter.X * (1.f - TargetSearchBoxHalfWidthRate), BoxCenter.Y * (1.f - TargetSearchBoxHalfHeightRate));
	const FVector2D BoxMax(BoxCenter.X *(1.f + TargetSearchBoxHalfWidthRate), BoxCenter.Y * (1.f + TargetSearchBoxHalfHeightRate));
	const FBox2D SearchBox(BoxMin, BoxMax);

	// The pawn bounds point mapping
	const FVector BoundsPointMapping[8] =
	{
		FVector(1.f, 1.f, 1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(-1.f, -1.f, -1.f), 
	};

	// Find nearest target in search box on screen
	APawn* TargetPawn = nullptr;
	const float MaxDistanceSquared = CheckTargetMaxDistance * CheckTargetMaxDistance;
	const APawn* Possessed = GetPawn();
	float ShortestDistanceSquared = -1.f;
	if (GetWorld())
	{
		for (TActorIterator<APawn> PawnIter(GetWorld()); PawnIter; ++PawnIter)
		{
			if (IsValid(*PawnIter))
			{
				// Skip possessed pawn
				if (*PawnIter == Possessed)
				{
					continue;
				}

				// Skip pawn too far
				const float DistSquared = FVector::DistSquared2D((*PawnIter)->GetActorLocation(), Possessed->GetActorLocation());
				if (DistSquared > MaxDistanceSquared)
				{
					continue;
				}
				
				// Check alive pawn
				const ADSCharacterBase* DSCharacter = Cast<ADSCharacterBase>(*PawnIter);
				if (nullptr == DSCharacter || !DSCharacter->IsAlive())
				{
					continue;
				}

				FVector BoundOrigin, BoundExtents;
				(*PawnIter)->GetActorBounds(true, BoundOrigin, BoundExtents);
				if (!BoundOrigin.IsNearlyZero() && !BoundExtents.IsNearlyZero())
				{
					FBox2D PawnBoundBox(ForceInit);
					bool bCompleteMakeBoundBox = true;
					for (uint8 Index = 0; Index < 8; ++Index)
					{
						FVector2D Projected;
						if (ProjectWorldLocationToScreen(BoundOrigin + (BoundsPointMapping[Index] * BoundExtents), Projected))
						{
							PawnBoundBox += Projected;
						}
						else
						{
							bCompleteMakeBoundBox = false;
							break;
						}
					}

					if (bCompleteMakeBoundBox)
					{
						if (SearchBox.Intersect(PawnBoundBox))
						{
							if (ShortestDistanceSquared < 0.f || ShortestDistanceSquared > DistSquared)
							{
								ShortestDistanceSquared = DistSquared;
								TargetPawn = (*PawnIter);
							}
						}
					}
				}
			}
		}
	}

	return TargetPawn;
}

void ADSPlayerControllerBase::SetTarget(APawn * NewTarget)
{
	// TODO : Check target alive or dead
	if (IsValid(NewTarget) && DSPlayerCameraManager)
	{
		DSPlayerCameraManager->SetTarget(NewTarget);
		ADSCharacterBase* Possessed = GetPawn<ADSCharacterBase>();
		if (IsValid(Possessed))
		{
			Possessed->OnOwnerLockedOnTarget();
		}
	}
}

void ADSPlayerControllerBase::CheckTargetState()
{
	if (DSPlayerCameraManager && IsValid(GetPawn()))
	{
		DSPlayerCameraManager->CheckTargetState(CheckTargetMaxDistance, GetPawn()->GetActorLocation());
	}
}

bool ADSPlayerControllerBase::IsTargeting() const
{
	if (DSPlayerCameraManager)
	{
		return DSPlayerCameraManager->IsTargeting();
	}

	return false;
}

void ADSPlayerControllerBase::LockOnTarget()
{
	if (IsTargeting())
	{
		ReleaseTarget();
	}
	else
	{
		APawn* Target = GetNearestTargetOnScreen();
		SetTarget(Target);
	}
}

void ADSPlayerControllerBase::ReleaseTarget()
{
	if (DSPlayerCameraManager)
	{
		DSPlayerCameraManager->ReleaseTarget();
		ADSCharacterBase* Possessed = GetPawn<ADSCharacterBase>();
		if (IsValid(Possessed))
		{
			Possessed->OnOwnerReleasedTarget();
		}
	}
}

// For camera shake test.
void ADSPlayerControllerBase::OnAttackHit()
{
	if (DSPlayerCameraManager)
	{
		DSPlayerCameraManager->StartCameraShake(AttackHitCameraShakeClass);
	}
}

