// All rights reserve SDH (2021 ~ )


#include "DSCharacterBase.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "DSCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DSPlayerControllerBase.h"

FName ADSCharacterBase::SpringArmComponentName = TEXT("SpringArm");
FName ADSCharacterBase::CameraComponentName = TEXT("Camera");

// Sets default values
ADSCharacterBase::ADSCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UDSCharacterMovementComponent>(CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(SpringArmComponentName);
	if (SpringArm)
	{
		SpringArm->SetupAttachment(GetCapsuleComponent());

		Camera = CreateDefaultSubobject<UCameraComponent>(CameraComponentName);
		if (Camera)
		{
			Camera->SetupAttachment(SpringArm);
		}
	}

}

// Called when the game starts or when spawned
void ADSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UDSCharacterMovementComponent* NewDSMovement = Cast<UDSCharacterMovementComponent>(GetCharacterMovement());
	if (NewDSMovement)
	{
		DSMovement = NewDSMovement;
	}

}

// Called every frame
void ADSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADSCharacterBase::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADSCharacterBase::MoveRight);
		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADSCharacterBase::Turn);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADSCharacterBase::LookUp);

		PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::StartJump);
		PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ADSCharacterBase::EndJump);
		PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::ToggleCrouch);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::Sprint, true);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Released, this, &ADSCharacterBase::Sprint, false);
	}

}

void ADSCharacterBase::SetSprinting(bool bSprint)
{
	if (IsValid(DSMovement))
	{
		if (HasAuthority())
		{
			DSMovement->SetSprinting(bSprint);
		}
		else
		{
			DSMovement->DoSprint(bSprint);
		}
	}
}

void ADSCharacterBase::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Dir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		AddMovementInput(Dir * Value);
	}
}

void ADSCharacterBase::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Dir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dir * Value);
	}
}

void ADSCharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ADSCharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ADSCharacterBase::StartJump()
{
	if (!bPressedJump)
	{
		Jump();
		if (IsValid(DSMovement))
		{
			DSMovement->bOrientRotationToMovement = false;
		}
	}
}

void ADSCharacterBase::EndJump()
{
	StopJumping();
}

void ADSCharacterBase::Sprint(bool bSprint)
{
	if (!bIsCrouched)
	{
		SetSprinting(bSprint);
		if (IsNetMode(NM_Client))
		{
			if (IsValid(SpringArm))
			{
				if (bSprint)
				{
					SpringArm->CameraLagMaxDistance += 100.f;
				}
				else
				{
					SpringArm->CameraLagMaxDistance -= 100.f;
				}
			}
		}
	}
}

void ADSCharacterBase::ToggleCrouch()
{
	if (!bIsCrouched)
	{
		Sprint(false);
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

bool ADSCharacterBase::IsSprinting() const
{
	if (IsValid(DSMovement))
	{
		return DSMovement->IsSprinting();
	}

	return false;
}
