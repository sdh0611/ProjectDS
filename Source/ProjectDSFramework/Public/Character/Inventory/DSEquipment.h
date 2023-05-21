// SDH, All rights reserved. (2021 ~ )

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
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Always called immediately after properties are received from the remote. */
	virtual void PostNetReceive() override;
	virtual void GivenTo(ADSCharacterBase* NewOwner) override;
	void Equipped();
	void Unequipped();
	FVector GetSocketLocation(const FName& SocketName);
	const class UStaticMeshSocket* GetSocket(const FName& SocketName) const;
	bool GetSocketRelativeLocationAndRotation(const FName& SocketName, FVector& OutLocation, FRotator& OutRotation);
	FVector GetSocketRelativeLocation(const FName& SocketName);
	FRotator GetSocketRelativeRotation(const FName& SocketName);

protected:
	virtual void InternalDiscard() override;
	virtual void InternalEquipped();
	virtual void InternalUnequipped();
	virtual void OnEquipped();
	virtual void OnUnequipped();

public:
	UStaticMeshComponent* GetBodyMesh() const { return BodyMesh; }
	bool IsEquipped() const { return bEquipped; }

public:
	void AttachToCharacter(const FName& AttackSocketName);
	void DetachFromCharacter();

	UFUNCTION()
	virtual void OnRep_Equipped();

public:
	static FName BodyMeshName;

public:
	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BodyMesh;

private:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Equipped)
	uint8 bEquipped : 1;
	
};
