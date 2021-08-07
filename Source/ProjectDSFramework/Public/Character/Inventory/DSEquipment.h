// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/DSInventoryEntity.h"
#include "DSEquipment.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS(Abstract, NotPlaceable)
class PROJECTDSFRAMEWORK_API ADSEquipment : public ADSInventoryEntity
{
	GENERATED_BODY()
	
public:
	ADSEquipment();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Equipped(ADSCharacterBase* EquipCharacter);
	virtual void Unequipped();

public:
	UStaticMeshComponent* GetBodyMesh() const { return BodyMesh; }
	bool IsEquipped() const { return bEquipped; }

public:
	static FName BodyMeshName;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* BodyMesh;

protected:
	uint8 bEquipped : 1;
	
};
