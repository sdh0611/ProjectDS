// SDH, All rights reserved. (2021 ~ )


#include "DSPlayerControllerBase.h"
#include "Components/InputComponent.h"
#include "DSGameplayStatics.h"
#include "DSPlayerCameraManager.h"
#include "DSCharacterBase.h"
#include "EngineUtils.h"

const uint16 ADSPlayerControllerBase::RotationInputFlag = ADSCharacterBase::EActiveInputFlag::InputTurn | ADSCharacterBase::EActiveInputFlag::InputLookUp;

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

void ADSPlayerControllerBase::LockOnTarget()
{
	if (IsTargeting())
	{
		ReleaseTarget();
	}
	else
	{
		APawn* Target = GetTargetOnScreen();
		SetTarget(Target);
	}
}

APawn * ADSPlayerControllerBase::GetTargetOnScreen()
{
	// Test code
	// TODO : Check screen rendered
	APawn* TargetPawn = nullptr;

	if (GetWorld())
	{
		for (TActorIterator<APawn> PawnIter(GetWorld()); PawnIter; ++PawnIter)
		{
			if (*PawnIter != GetPawn())
			{
				TargetPawn = *PawnIter;
				break;
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
			Possessed->DisableCharacterInput(RotationInputFlag);
		}
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
			Possessed->EnableCharacterInput(RotationInputFlag);
		}
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
