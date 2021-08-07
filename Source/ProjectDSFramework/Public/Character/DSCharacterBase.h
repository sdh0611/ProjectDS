// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSCharacterBase.generated.h"

class UDSCharacterMovementComponent;
class ADSWeapon;

UCLASS()
class PROJECTDSFRAMEWORK_API ADSCharacterBase : public ACharacter
{
	GENERATED_BODY()

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

public:
	FORCEINLINE UDSCharacterMovementComponent* GetDSCharacterMovement() const { return DSMovement; }
	FORCEINLINE ADSWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	bool IsArmed() const;
	bool IsSprinting() const;

public:
	float PlayMontage(class UAnimMontage* MontageToPlay, float PlayRate = 1.f, float StartPosition = 0.f, bool bStopAllMontage = false, float BlendOutTime = 0.f);

protected:
	void SetSprinting(bool bSprint);

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void StartJump();
	void EndJump();
	void Sprint(bool bSprint);
	void ToggleCrouch();
	void ToggleWeapon();

protected:
	virtual void EquipWeapon(ADSWeapon* Equipped);
	virtual void UnequipWeapon();
	void ActivateWeapon();
	void DeactivateWeapon();

public:
	static FName SpringArmComponentName;
	static FName CameraComponentName;

protected:
	// {{ Input binding delegates
	DECLARE_DELEGATE_OneParam(FActionInputDelegate, bool);
	// }} Input binding delegates

public:
	// For weapon test
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADSWeapon> TestWeaponClass;

protected:
	UPROPERTY(VisibleAnywhere, Category=Inventory)
	ADSWeapon* CurrentWeapon;

	UPROPERTY()
	class UDSCharacterAnimInstance* DSAnimInstance;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(Transient)
	class UDSCharacterMovementComponent* DSMovement;

};
