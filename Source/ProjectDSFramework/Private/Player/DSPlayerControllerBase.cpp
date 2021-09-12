// SDH, All rights reserved. (2021 ~ )


#include "DSPlayerControllerBase.h"
#include "Components/InputComponent.h"
#include "DSGameplayStatics.h"
#include "DSPlayerCameraManager.h"

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
		InputComponent->BindAction<FControllerActionInputDelegate>(TEXT("LockOnTarget"), EInputEvent::IE_Pressed, this, &ADSPlayerControllerBase::LockOnTarget, true);
		InputComponent->BindAction<FControllerActionInputDelegate>(TEXT("LockOnTarget"), EInputEvent::IE_Released, this, &ADSPlayerControllerBase::LockOnTarget, false);
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

void ADSPlayerControllerBase::LockOnTarget(bool bLockOn)
{
	if (bLockOn)
	{
		APawn* Target = GetTargetOnScreen();
		SetTarget(Target);
	}
	else
	{
		ReleaseTarget();
	}	
}

APawn * ADSPlayerControllerBase::GetTargetOnScreen()
{


	return nullptr;
}

void ADSPlayerControllerBase::SetTarget(APawn * NewTarget)
{
	// TODO : Check target alive or dead
	if (IsValid(NewTarget) && DSPlayerCameraManager)
	{
		DSPlayerCameraManager->SetTarget(NewTarget);
	}
}

void ADSPlayerControllerBase::ReleaseTarget()
{
	if (DSPlayerCameraManager)
	{
		DSPlayerCameraManager->ReleaseTarget();
	}
}
