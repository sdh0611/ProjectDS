// SDH, All rights reserved. (2021 ~ )


#include "DSCharacterBase.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "DSCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DSPlayerControllerBase.h"
#include "DSWeapon.h"
#include "DSCharacterAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "DSCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

FName ADSCharacterBase::SpringArmComponentName = TEXT("SpringArm");
FName ADSCharacterBase::CameraComponentName = TEXT("Camera");
FName ADSCharacterBase::CharacterStatComponentName = TEXT("CharacterStat");
const uint16 ADSCharacterBase::RotationInputFlag = ADSCharacterBase::EActiveInputFlag::InputTurn | ADSCharacterBase::EActiveInputFlag::InputLookUp;

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

	CharacterStat = CreateDefaultSubobject<UDSCharacterStatComponent>(CharacterStatComponentName);

	ActiveMoveInputFlag = EActiveInputFlag::InputAll;
	bTargeting = false;
}

// Called when the game starts or when spawned
void ADSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// {{ Begin test weapon 
	if (HasAuthority() && TestWeaponClass && GetWorld())
	{
		ADSWeapon* TestWeapon = nullptr;
		FActorSpawnParameters WeaponSpawnParams;
		WeaponSpawnParams.Instigator = this;
		WeaponSpawnParams.Owner = this;
		WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		TestWeapon = GetWorld()->SpawnActor<ADSWeapon>(TestWeaponClass, WeaponSpawnParams);
		if (TestWeapon)
		{
			TestWeapon->GivenTo(this);
			EquipWeapon(TestWeapon);
		}
	}
	// }} End test weapon 

}

void ADSCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UDSCharacterMovementComponent* NewDSMovement = Cast<UDSCharacterMovementComponent>(GetCharacterMovement());
	if (NewDSMovement)
	{
		DSMovement = NewDSMovement;
	}

	if (GetMesh())
	{
		UDSCharacterAnimInstance* NewAnim = Cast<UDSCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (NewAnim)
		{
			DSAnimInstance = NewAnim;
		}
	}

}

// Called every frame
void ADSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugBox(GetWorld(), GetActorLocation(), FVector(20.f), FColor::Yellow);
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
		PlayerInputComponent->BindAction(TEXT("ToggleWeapon"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::ToggleWeapon);
		PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::Attack);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::Sprint, true);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Released, this, &ADSCharacterBase::Sprint, false);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Walk"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::Walk, true);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Walk"), EInputEvent::IE_Released, this, &ADSCharacterBase::Walk, false);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Guard"), EInputEvent::IE_Pressed, this, &ADSCharacterBase::Guard, true);
		PlayerInputComponent->BindAction<FActionInputDelegate>(TEXT("Guard"), EInputEvent::IE_Released, this, &ADSCharacterBase::Guard, false);
	}

}

void ADSCharacterBase::Falling()
{
	Super::Falling();

	const uint16 InputFlagToDIsable = (EActiveInputFlag::InputJump | EActiveInputFlag::InputEquipWeapon);
	DisableCharacterInput(InputFlagToDIsable);
	if (DSMovement)
	{
		DSMovement->bOrientRotationToMovement = false;
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

void ADSCharacterBase::SetWalking(bool bWalk)
{
	if (IsValid(DSMovement))
	{
		if (HasAuthority())
		{
			DSMovement->SetWalking(bWalk);
		}
		else
		{
			DSMovement->DoWalk(bWalk);
		}
	}
}

void ADSCharacterBase::MoveForward(float Value)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputMoveForward))
	{
		if (Controller && Value != 0.f)
		{
			const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Dir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
			AddMovementInput(Dir * Value);
		}
	}
}

void ADSCharacterBase::MoveRight(float Value)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputMoveRight))
	{
		if (Controller && Value != 0.f)
		{
			const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Dir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
			AddMovementInput(Dir * Value);
		}
	}
}

void ADSCharacterBase::Turn(float Value)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputTurn))
	{
		AddControllerYawInput(Value);
	}
}

void ADSCharacterBase::LookUp(float Value)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputLookUp))
	{
		AddControllerPitchInput(Value);
	}
}

void ADSCharacterBase::StartJump()
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputJump) && !bPressedJump)
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
	if (IsMoveInputAllowed(EActiveInputFlag::InputSprint) || bSprint)
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

void ADSCharacterBase::Walk(bool bWalk)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputWalk) || !bWalk)
	{
		SetWalking(bWalk);
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

void ADSCharacterBase::ToggleWeapon()
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputEquipWeapon))
	{
		if (IsValid(CurrentWeapon) && CurrentWeapon->IsEquipped())
		{
			if (HasAuthority())
			{
				CurrentWeapon->SetWeaponArmed(!CurrentWeapon->IsWeaponArmed());
			}
			else
			{
				ServerToggleWeapon();
			}
		}
	}
}

void ADSCharacterBase::Attack()
{
	// Contain autonomous proxy, authority
	if (GetLocalRole() > ROLE_SimulatedProxy)
	{
		if (IsMoveInputAllowed(EActiveInputFlag::InputAttack))
		{
			if (IsValid(CurrentWeapon) && CurrentWeapon->CanAttack())
			{
				CurrentWeapon->TryAttack();
				if (IsNetMode(NM_Client))
				{
					ServerDoAttack();
				}
			}
		}
	}
}

void ADSCharacterBase::Guard(bool bGuard)
{


}

void ADSCharacterBase::EquipWeapon(ADSWeapon * Equipped)
{
	if (IsValid(Equipped))
	{
		// Unequip current weapon
		UnequipWeapon();

		// Equip new weapon
		Equipped->Equipped();
		CurrentWeapon = Equipped;
		ArmWeapon();
	}
}

void ADSCharacterBase::UnequipWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Unequipped();
		CurrentWeapon = nullptr;
	}
}

void ADSCharacterBase::ArmWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		// Activate weapon
		CurrentWeapon->WeaponArmed();
	}
}

void ADSCharacterBase::UnarmWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		// Unarmed weapon
		CurrentWeapon->WeaponUnarmed();
	}
}

void ADSCharacterBase::OnRep_CurrentWeapon()
{
	ArmWeapon();
}

bool ADSCharacterBase::ServerLockOnTarget_Validate(bool bLockOn)
{
	return true;
}

void ADSCharacterBase::ServerLockOnTarget_Implementation(bool bLockOn)
{
	if (bLockOn)
	{
		OnOwnerLockedOnTarget();
	}
	else
	{
		OnOwnerReleasedTarget();
	}
}

void ADSCharacterBase::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);

	const uint16 MoveInputToEnable = (EActiveInputFlag::InputJump | EActiveInputFlag::InputEquipWeapon);
	EnableCharacterInput(MoveInputToEnable);

	ADSPlayerControllerBase* Possessor = GetController<ADSPlayerControllerBase>();
	if (IsValid(Possessor) && !Possessor->IsTargeting())
	{
		if (DSMovement)
		{
			DSMovement->bOrientRotationToMovement = false;
		}
	}
}

void ADSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADSCharacterBase, CurrentWeapon);
	DOREPLIFETIME_CONDITION(ADSCharacterBase, bTargeting, COND_SkipOwner);
}

float ADSCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogClass, Warning, TEXT("[TakeDamageLog] Take damage !!!"));

	return FinalDamage;
}

bool ADSCharacterBase::IsArmed() const
{
	return IsValid(CurrentWeapon) && CurrentWeapon->IsWeaponArmed();
}

bool ADSCharacterBase::IsSprinting() const
{
	if (IsValid(DSMovement))
	{
		return DSMovement->IsSprinting();
	}

	return false;
}

bool ADSCharacterBase::IsWalking() const
{
	if(IsValid(DSMovement))
	{
		return DSMovement->IsWalking();
	}

	return false;
}

bool ADSCharacterBase::IsTargeting() const
{
	return bTargeting;
}

bool ADSCharacterBase::IsMoveInputAllowed(uint16 TestInput) const
{
	return GetActiveMoveInputFlag() & TestInput;
}

void ADSCharacterBase::SetCharacterInputFlag(uint16 NewFlag)
{
	ActiveMoveInputFlag = EActiveInputFlag(NewFlag);
}

void ADSCharacterBase::DisableCharacterInput(uint16 ExcludeFlag)
{
	if (ExcludeFlag <= ActiveMoveInputFlag)
	{ 
		SetCharacterInputFlag(ActiveMoveInputFlag & (~ExcludeFlag));
	}
}

void ADSCharacterBase::EnableCharacterInput(uint16 IncludeFlag)
{
	SetCharacterInputFlag(ActiveMoveInputFlag | IncludeFlag);
}

void ADSCharacterBase::OnOwnerLockedOnTarget()
{
	if (IsLocallyControlled())
	{
		// Notify to authority from autonomous proxy
		ServerLockOnTarget(true);
	}

	DisableCharacterInput(RotationInputFlag);

	if (DSMovement)
	{
		DSMovement->bOrientRotationToMovement = false;
	}

	bTargeting = true;
}

void ADSCharacterBase::OnOwnerReleasedTarget()
{
	if (IsLocallyControlled())
	{
		// Notify to authority from autonomous proxy
		ServerLockOnTarget(false);
	}

	EnableCharacterInput(RotationInputFlag);

	if (DSMovement)
	{
		DSMovement->bOrientRotationToMovement = true;
	}

	bTargeting = false;
}

bool ADSCharacterBase::WasCharacterRecentlyRendered(float Tolerance) const
{
	if (GetMesh())
	{
		return GetMesh()->WasRecentlyRendered(Tolerance);
	}

	return WasRecentlyRendered(Tolerance);
}

float ADSCharacterBase::PlayMontage(UAnimMontage * MontageToPlay, float PlayRate, float StartPosition, bool bStopAllMontage, float BlendOutTime)
{
	float PlayMontageLength = 0.f;
	if (IsValid(DSAnimInstance) && MontageToPlay)
	{
		if (bStopAllMontage)
		{
			DSAnimInstance->StopAllMontages(BlendOutTime);
		}

		PlayMontageLength = DSAnimInstance->Montage_Play(MontageToPlay, PlayRate, EMontagePlayReturnType::MontageLength, StartPosition);
	}

	return PlayMontageLength;
}

bool ADSCharacterBase::ServerEquipWeapon_Validate(class ADSWeapon* NewWeapon)
{
	return true;
}

void ADSCharacterBase::ServerEquipWeapon_Implementation(class ADSWeapon* NewWeapon)
{
	EquipWeapon(NewWeapon);
}

bool ADSCharacterBase::ServerEquipEquipment_Validate(class ADSEquipment* NewEquipment)
{
	return true;
}

void ADSCharacterBase::ServerEquipEquipment_Implementation(class ADSEquipment* NewEquipment)
{


}

bool ADSCharacterBase::ServerToggleWeapon_Validate()
{
	return true;
}

void ADSCharacterBase::ServerToggleWeapon_Implementation()
{
	if (IsValid(CurrentWeapon) && CurrentWeapon->IsEquipped())
	{
		CurrentWeapon->SetWeaponArmed(!CurrentWeapon->IsWeaponArmed());
	}
}

bool ADSCharacterBase::ServerDoAttack_Validate()
{
	return true;
}

void ADSCharacterBase::ServerDoAttack_Implementation()
{
	Attack();
}