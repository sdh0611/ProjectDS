// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSCharacterBase.generated.h"

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	// {{ Input binding delegates
	DECLARE_DELEGATE_OneParam(FActionInputDelegate, bool);
	// }} Input binding delegates

private:
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetSprint(bool bSprint);

public:
	static FName SpringArmComponentName;
	static FName CameraComponentName;

private:
	uint8 bCrouching : 1;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

};
