// All rights reserve SDH (2021 ~ )


#include "DSInventoryEntity.h"
#include "DSCharacterBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADSInventoryEntity::ADSInventoryEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADSInventoryEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSInventoryEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ADSInventoryEntity, OwnerCharacter, COND_OwnerOnly);
	DOREPLIFETIME(ADSInventoryEntity, OwnerCharacter);
}

void ADSInventoryEntity::GivenTo(ADSCharacterBase * NewOwner)
{
	OwnerCharacter = NewOwner;
}

void ADSInventoryEntity::Discard()
{
	InternalDiscard();

	OwnerCharacter.Reset();
}

void ADSInventoryEntity::InternalDiscard()
{
}
