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
#include "EnhancedInputComponent.h"
#include "Input/DSInputSetting.h"
#include "DSGameplayStatics.h"

const FName ADSCharacterBase::SpringArmComponentName = TEXT("SpringArm");
const FName ADSCharacterBase::CameraComponentName = TEXT("Camera");
const FName ADSCharacterBase::CharacterStatComponentName = TEXT("CharacterStat");
const uint16 ADSCharacterBase::RotationInputFlag = ADSCharacterBase::EActiveInputFlag::InputTurn | ADSCharacterBase::EActiveInputFlag::InputLookUp;
const FName ADSCharacterBase::PhysicsSimulationStartBoneName = TEXT("pelvis");

// Sets default values
ADSCharacterBase::ADSCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDSCharacterMovementComponent>(CharacterMovementComponentName))
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

	MaxHealth = 100;
	Health = MaxHealth;
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

	// Test
	Health = MaxHealth;
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
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		ApplyCharacterInputMappingContext();
		BindInputAction(EnhancedInputComponent);
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
		DSMovement->bUseControllerDesiredRotation = false;
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

void ADSCharacterBase::SetHealth(int32 NewHealth)
{
	Health = FMath::Clamp<int32>(NewHealth, 0.f, MaxHealth);
}

void ADSCharacterBase::ActiveRagdoll()
{
	if (GetMesh())
	{
		//GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PhysicsSimulationStartBoneName, true, true);
		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

}

void ADSCharacterBase::DeactiveRagdoll()
{
	if (GetMesh())
	{
		//GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ADSCharacterBase::ApplyCharacterInputMappingContext()
{
	if (IsValid(CharacterInputSetting) && CharacterInputSetting->GetInputMappingContextSet().IsValidSet())
	{
		// Enhanced input system
		ADSPlayerControllerBase* DSPC = GetController<ADSPlayerControllerBase>();
		if (IsValid(DSPC))
		{
			DSPC->ApplyInputMappingContext(CharacterInputSetting->GetInputMappingContextSet().InputMappingContext, CharacterInputSetting->GetInputMappingContextSet().Priority);
		}
	}
}

void ADSCharacterBase::BindInputAction(UEnhancedInputComponent* InInputComponent)
{
	if (InInputComponent && IsValid(CharacterInputSetting))
	{
#define BIND_CHARACTER_ACTION_INPUT(InputFunctionName, TriggerEvent)	\
			static const FName InputFunctionName##InputName = FName(TEXT(#InputFunctionName));	\
			if (UInputAction* TargetInputAction = CharacterInputSetting->GetInputAction(InputFunctionName##InputName))	\
			{\
				InInputComponent->BindAction(TargetInputAction, TriggerEvent, this, &ADSCharacterBase::InputFunctionName);\
			}

		BIND_CHARACTER_ACTION_INPUT(Movement, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(TurnAndLookUp, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(Jump, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(Sprint, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(Walk, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(ToggleCrouch, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(Attack, ETriggerEvent::Triggered)
		BIND_CHARACTER_ACTION_INPUT(AltAttack, ETriggerEvent::Triggered)

#undef BIND_CHARACTER_ACTION_INPUT
	}

}

void ADSCharacterBase::Movement(const FInputActionValue& InActionValue)
{
	if (IsValid(Controller))
	{
		const FVector2D InputValue = InActionValue.Get<FVector2D>();
		const FRotator ControlYawRot(0.f, GetControlRotation().Yaw, 0.f);
		if (IsMoveInputAllowed(EActiveInputFlag::InputMoveForward) && InputValue.Y != 0.f)
		{
			AddMovementInput(ControlYawRot.RotateVector(FVector::ForwardVector), InputValue.Y);
		}

		if (IsMoveInputAllowed(EActiveInputFlag::InputMoveRight) && InputValue.X != 0.f)
		{
			AddMovementInput(ControlYawRot.RotateVector(FVector::RightVector), InputValue.X);
		}
	}
}

void ADSCharacterBase::TurnAndLookUp(const FInputActionValue& InActionValue)
{
	const FVector2D InputValue = InActionValue.Get<FVector2D>();
	if (IsMoveInputAllowed(EActiveInputFlag::InputTurn) && InputValue.X != 0.f)
	{
		AddControllerYawInput(InputValue.X);
	}

	if (IsMoveInputAllowed(EActiveInputFlag::InputLookUp) && InputValue.Y != 0.f)
	{
		AddControllerPitchInput(InputValue.Y);
	}
}

void ADSCharacterBase::Sprint(const FInputActionValue& InActionValue)
{
	const bool bPressed = InActionValue.Get<bool>();

	if (IsMoveInputAllowed(EActiveInputFlag::InputSprint) || !bPressed)
	{
		SetSprinting(bPressed);
		if (IsNetMode(NM_Client))
		{
			if (IsValid(SpringArm))
			{
				if (bPressed)
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

void ADSCharacterBase::Walk(const FInputActionValue& InActionValue)
{
	const bool bPressed = InActionValue.Get<bool>();
	if (IsMoveInputAllowed(EActiveInputFlag::InputWalk) || !bPressed)
	{
		SetWalking(bPressed);
	}
}

void ADSCharacterBase::ToggleCrouch(const FInputActionValue& InActionValue)
{
	const bool bPressed = InActionValue.Get<bool>();
	if (bPressed)
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
}

void ADSCharacterBase::DoJump(const FInputActionValue& InActionValue)
{
	if (IsMoveInputAllowed(EActiveInputFlag::InputJump))
	{
		const bool bPressed = InActionValue.Get<bool>();
		if (bPressed)
		{
			Jump();
		}
	}
}

void ADSCharacterBase::ToggleWeapon(const FInputActionValue& InActionValue)
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

void ADSCharacterBase::Attack(const FInputActionValue& InActionValue)
{
	TryAttack(EWeaponActionInput::Attack);
}

void ADSCharacterBase::AltAttack(const FInputActionValue& InActionValue)
{
	const bool bPressed = InActionValue.Get<bool>();
	if (bPressed)
	{
		TryAttack(EWeaponActionInput::AltAttack);
	}
	else
	{
		TryAttack(EWeaponActionInput::AltAttackRelease);
	}
}

void ADSCharacterBase::TryAttack(EWeaponActionInput AttackType)
{
	// Contain autonomous proxy, authority
	if (GetLocalRole() > ROLE_SimulatedProxy)
	{
		if (IsMoveInputAllowed(EActiveInputFlag::InputAttack))
		{
			if (IsValid(CurrentWeapon) && CurrentWeapon->CanHandleWeaponActionInput(AttackType))
			{
				CurrentWeapon->HandleWeaponActionInput(AttackType);
				if (IsNetMode(NM_Client))
				{
					ServerTryAttack(AttackType);
				}
			}
		}
	}

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

void ADSCharacterBase::OnAttackStart()
{
	// Attack, Move 제외한 모든 입력을 막음.
	uint16 NewInput = ADSCharacterBase::EActiveInputFlag::InputAttack | ADSCharacterBase::EActiveInputFlag::InputMoveForward | ADSCharacterBase::EActiveInputFlag::InputMoveRight;
	if (!IsTargeting())
	{
		NewInput |= RotationInputFlag;
	}

	SetCharacterInputFlag(NewInput);

	// Sprint 취소
	if (IsSprinting())
	{
		Sprint(false);
	}

	//RotateToDesired();
}

void ADSCharacterBase::OnAttackEnd()
{
	SetCharacterInputFlag(ADSCharacterBase::EActiveInputFlag::InputAll);

	// Targeting중이면 LookUp, Turn 입력 막음.
	if (IsTargeting())
	{
		const uint16 DisableInput = RotationInputFlag;
		DisableCharacterInput(DisableInput);
	}
}

void ADSCharacterBase::ProcessHit(const FTakeHitInfo & PlayHitInfo)
{
	// Test code
	if (HitReactAnims.Num() > 0)
	{
		PlayAnimMontage(HitReactAnims[0].HitReactAnim);

		// For hit stop test
		OnAttackHit();
	}
}

void ADSCharacterBase::SimulateTakeDamage()
{
}

void ADSCharacterBase::Die()
{
	if (IsLocallyControlled())
	{
		SetCharacterInputFlag(RotationInputFlag);
	}

	ActiveRagdoll();
}

void ADSCharacterBase::OnRep_CurrentWeapon()
{
	ArmWeapon();
}

void ADSCharacterBase::OnRep_HitInfo()
{
	UE_LOG(LogClass, Warning, TEXT("[HitDebugLog] HitInfo repliocated, damage : %f"), HitInfo.GetDamage());
	ProcessHit(HitInfo);
}

void ADSCharacterBase::OnRep_Health()
{
	UE_LOG(LogClass, Warning, TEXT("[HealthDebugLog] Health replicated : %d"), Health);
	if (!IsAlive())
	{
		UE_LOG(LogClass, Warning, TEXT("[HealthDebugLog] Die...."));
		Die();
	}
}

void ADSCharacterBase::OnRep_MaxHealth()
{
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

	if (DSMovement)
	{
		DSMovement->bUseControllerDesiredRotation = true;
		DSMovement->bOrientRotationToMovement = !IsTargeting();
	}
}

void ADSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADSCharacterBase, CurrentWeapon);
	DOREPLIFETIME(ADSCharacterBase, HitInfo);
	DOREPLIFETIME(ADSCharacterBase, Health);
	DOREPLIFETIME(ADSCharacterBase, MaxHealth);
	
	DOREPLIFETIME_CONDITION(ADSCharacterBase, bTargeting, COND_SkipOwner);
}

float ADSCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = 0.f;

	if (!HasAuthority())
	{
		SimulateTakeDamage();
	}
	else
	{
		// Calc damage
		FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

		// Apply damage
		SetHealth(GetHealth() - static_cast<int32>(FinalDamage));

		// Check died
		if (!IsAlive())
		{
			Die();
		}
		else
		{
			// Replication test code
			HitInfo.SetDamage(FinalDamage);

			if (IsNetMode(NM_Standalone) || IsNetMode(NM_ListenServer))
			{
				OnRep_HitInfo();
			}
		}
	}

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

bool ADSCharacterBase::IsAlive() const
{
	return Health > 0;
}

float ADSCharacterBase::GetHealth() const
{
	return Health;
}

FVector ADSCharacterBase::GetWeaponSocketLocation(const FName & SocketName) const
{
	FVector SocketLocation = FVector::ZeroVector;

	if (IsValid(CurrentWeapon) && CurrentWeapon->IsWeaponArmed())
	{
		SocketLocation = CurrentWeapon->GetSocketLocation(SocketName);
	}

	return SocketLocation;
}

FVector ADSCharacterBase::GetWeaponLeftHandIKEffectorLocation(const FName & SocketName) const
{
	FVector SocketLocation = FVector::ZeroVector;

	if (IsValid(CurrentWeapon) && CurrentWeapon->IsWeaponArmed())
	{
		SocketLocation = CurrentWeapon->GetSocketLocation(SocketName);
		if (GetMesh())
		{
			const FVector Offset = GetMesh()->GetSocketLocation(TEXT("hand_l")) - GetMesh()->GetSocketLocation(TEXT("ik_weapon_hand_l"));
			SocketLocation += Offset;
		}
		//SocketLocation = CurrentWeapon->GetSocketRelativeLocation(SocketName);
	}

	return SocketLocation;
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
	// Prevent recursive function call 
	if (IsLocallyControlled() && !HasAuthority())
	{
		// Notify to authority from autonomous proxy
		ServerLockOnTarget(true);
	}

	DisableCharacterInput(RotationInputFlag);

	if (DSMovement)
	{
		DSMovement->bOrientRotationToMovement = false;
		DSMovement->bUseControllerDesiredRotation = true;
	}

	bTargeting = true;
}

void ADSCharacterBase::OnOwnerReleasedTarget()
{
	// Prevent recursive function call 
	if (IsLocallyControlled() && !HasAuthority())
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

void ADSCharacterBase::RotateToDesired()
{
	APlayerController* PC = GetController<APlayerController>();
	if (IsValid(PC))
	{
		const FRotator DesiredRot(0.f, PC->GetDesiredRotation().Yaw, 0.f);
		SetActorRotation(DesiredRot);
	}
}

void ADSCharacterBase::OnAttackHit()
{
	// Test code
	if (DSAnimInstance)
	{
		//GetMesh()->SetComponentTickEnabled(false);
		//GetMesh()->SetComponentTickInterval(0.1f);
		DSAnimInstance->Montage_Pause(NULL);

		TWeakObjectPtr<ADSCharacterBase> WeakThis(this);
		auto HitStopTest = [WeakThis](){
			ADSCharacterBase* Self = WeakThis.Get();
			if (IsValid(Self) && Self->DSAnimInstance)
			{
				//Self->GetMesh()->SetComponentTickEnabled(true);
				//Self->GetMesh()->SetComponentTickInterval(0.f);
				Self->DSAnimInstance->Montage_Resume(nullptr);
			}
		};

		GetWorldTimerManager().SetTimer(HitStopTimer, HitStopTest, 0.1f, false);
	}

	ADSPlayerControllerBase* DSPC = GetController<ADSPlayerControllerBase>();
	if (DSPC)
	{
		DSPC->OnAttackHit();
	}
}

void ADSCharacterBase::SubscribeMontageEndedDelegate(FOnMontageEnded & InDelegate, UAnimMontage * AnimMontageToBind)
{
	if (GetMesh())
	{
		UAnimInstance* CharacterAnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_SetEndDelegate(InDelegate, AnimMontageToBind);
		}
	}

}

void ADSCharacterBase::UnsubscribeMontageEndedDelegate(UAnimMontage * AnimMontageToBind)
{
	if (GetMesh())
	{
		UAnimInstance* CharacterAnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(CharacterAnimInstance))
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = CharacterAnimInstance->GetActiveInstanceForMontage(AnimMontageToBind);
			if (MontageInstance)
			{
				MontageInstance->OnMontageEnded.Unbind();
			}
		}
	}

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

bool ADSCharacterBase::ServerTryAttack_Validate(EWeaponActionInput AttackType)
{
	return true;
}

void ADSCharacterBase::ServerTryAttack_Implementation(EWeaponActionInput AttackType)
{
	TryAttack(AttackType);
}