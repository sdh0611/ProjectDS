// All rights reserve SDH (2021 ~ )

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
	virtual void GivenTo(ADSCharacterBase* NewOwner);
	ADSCharacterBase* GetInventoryEntityOwner() { return OwnerCharacter.Get(); }

protected:
	TWeakObjectPtr<ADSCharacterBase> OwnerCharacter;

};
