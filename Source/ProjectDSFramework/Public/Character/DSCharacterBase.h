// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSBaseTypes.h"
#include "DSCharacterBase.generated.h"

class UDSCharacterMovementComponent;
class ADSWeapon;
class ADSEquipment;


USTRUCT(BlueprintType)
struct FDSCharacterHitReaction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitReactAnim;

	FDSCharacterHitReaction()
		: HitReactAnim(nullptr)
	{
	}

};


UCLASS(Abstract)
class PROJECTDSFRAMEWORK_API ADSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	enum EActiveInputFlag : uint16
	{
		InputNone = 0x0000,
		InputMoveForward = 0x0001,
		InputMoveRight = 0x0002,
		InputTurn = 0x0004,
		InputLookUp = 0x0008,
		InputJump  = 0x0010,
		InputSprint = 0x0020,
		InputWalk = 0x0040,
		InputEquipWeapon = 0x0080,
		InputAttack = 0x0100,
		InputAll = 0xffff
	};

public:
	// Sets default values for this character's properties
	ADSCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Called when the character's movement enters falling */
	virtual void Falling() override;
	/**
	 * Called upon landing when falling, to perform actions based on the Hit result. Triggers the OnLanded event.
	 * Note that movement mode is still "Falling" during this event. Current Velocity value is the velocity at the time of landing.
	 * Consider OnMovementModeChanged() as well, as that can be used once the movement mode changes to the new mode (most likely Walking).
	 *
	 * @param Hit Result describing the landing that resulted in a valid landing spot.
	 * @see OnMovementModeChanged()
	 */
	virtual void Landed(const FHitResult& Hit) override;
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/**
	 * Apply damage to this actor.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



public:
	FORCEINLINE UDSCharacterMovementComponent* GetDSCharacterMovement() const { return DSMovement; }
	FORCEINLINE ADSWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	template<typename T>
	T* GetCurrentWeapon()
	{
		return Cast<T>(CurrentWeapon);
	}

	bool IsArmed() const;
	bool IsSprinting() const;
	bool IsWalking() const;
	bool IsTargeting() const;
	EActiveInputFlag GetActiveMoveInputFlag() const { return ActiveMoveInputFlag; }
	bool IsMoveInputAllowed(uint16 TestInput) const;
	void SetCharacterInputFlag(uint16 NewFlag);
	void DisableCharacterInput(uint16 ExcludeFlag);
	void EnableCharacterInput(uint16 IncludeFlag);
	void OnOwnerLockedOnTarget();
	void OnOwnerReleasedTarget();
	bool WasCharacterRecentlyRendered(float Tolerance = KINDA_SMALL_NUMBER) const;
	void RotateToDesired();
	void OnAttackHit();
	void SubscribeMontageEndedDelegate(FOnMontageEnded& InDelegate, class UAnimMontage* AnimMontageToBind = nullptr);
	void UnsubscribeMontageEndedDelegate(class UAnimMontage* AnimMontageToBind = nullptr);

public:
	// Deprecated
	UE_DEPRECATED(21.10.11, "Use APawn::PlayAnimMontage instead.")
	float PlayMontage(class UAnimMontage* MontageToPlay, float PlayRate = 1.f, float StartPosition = 0.f, bool bStopAllMontage = false, float BlendOutTime = 0.f);

protected:
	void SetSprinting(bool bSprint);
	void SetWalking(bool bWalk);

// ~ Begin player input binds
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void StartJump();
	void EndJump();
	void Sprint(bool bSprint);
	void Walk(bool bWalk);
	void ToggleCrouch();
	void ToggleWeapon();
	void Attack();
	void Guard(bool bGuard);
// ~ End player input binds

public:
	virtual void EquipWeapon(ADSWeapon* Equipped);
	virtual void UnequipWeapon();
	void ArmWeapon();
	void UnarmWeapon();
	void OnAttackStart();
	void OnAttackEnd();

protected:
	void ProcessHit(const FTakeHitInfo& PlayHitInfo);
	void Die();

protected:
	UFUNCTION()
	virtual void OnRep_CurrentWeapon();

	UFUNCTION()
	virtual void OnRep_HitInfo();

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipWeapon(class ADSWeapon* NewEquipment);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipEquipment(class ADSEquipment* NewEquipment);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLockOnTarget(bool bLockOn);

public:
	static FName SpringArmComponentName;
	static FName CameraComponentName;
	static FName CharacterStatComponentName;
	static const uint16 RotationInputFlag;

protected:
	// {{ Input binding delegates
	DECLARE_DELEGATE_OneParam(FActionInputDelegate, bool);
	// }} Input binding delegates

public:
	// For weapon test
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADSWeapon> TestWeaponClass;

private:
	EActiveInputFlag ActiveMoveInputFlag;

	FTimerHandle HitStopTimer;

// ~ Begin replicated properties
protected:
	UPROPERTY(VisibleAnywhere, Category=Inventory, Transient, ReplicatedUsing=OnRep_CurrentWeapon)
	ADSWeapon* CurrentWeapon;

	UPROPERTY(Transient, Replicated)
	uint8 bTargeting : 1;

	// For replicate damage info
	UPROPERTY(Transient, ReplicatedUsing=OnRep_HitInfo)
	FTakeHitInfo HitInfo;

// ~ End replicated properties

protected:
	UPROPERTY()
	class UDSCharacterAnimInstance* DSAnimInstance;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UDSCharacterStatComponent* CharacterStat;

	UPROPERTY(Transient)
	class UDSCharacterMovementComponent* DSMovement;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TArray<FDSCharacterHitReaction> HitReactAnims;

};
