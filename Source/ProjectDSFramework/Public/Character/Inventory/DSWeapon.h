// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSEquipment.h"
#include "DSWeapon.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponAnimInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName AnimName;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* SlotAnim;

	UPROPERTY(EditAnywhere)
	float PlayRate;

	FWeaponAnimInfo()
		: AnimName(NAME_None)
		, SlotAnim(nullptr)
		, PlayRate(1.f)
	{
	}
};

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API ADSWeapon : public ADSEquipment
{
	GENERATED_BODY()
	
public:
	ADSWeapon();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Equipped(ADSCharacterBase* EquipCharacter) override;
	virtual void Unequipped() override;

public:
	void WeaponActivated();
	void WeaponDeactivated();

public:
	bool IsWeaponActive() const { return bWeaponActive; }

protected:
	void AttachWeapon();
	void DetachWeapon();

public:
	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketNameOnActivated;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketNameOnDeactivated;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* UnequipAnim;

protected:
	uint8 bWeaponActive : 1;

};
