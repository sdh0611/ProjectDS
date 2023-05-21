// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSInventoryEntity.generated.h"

class ADSCharacterBase;

UCLASS(NotPlaceable, Abstract)
class PROJECTDSFRAMEWORK_API ADSInventoryEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSInventoryEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void GivenTo(ADSCharacterBase* NewOwner);
	void Discard();
	ADSCharacterBase* GetInventoryEntityOwner() { return OwnerCharacter.Get(); }

protected:
	virtual void InternalDiscard();

protected:
	UPROPERTY(Transient, Replicated)
	TObjectPtr<ADSCharacterBase> OwnerCharacter;

};
