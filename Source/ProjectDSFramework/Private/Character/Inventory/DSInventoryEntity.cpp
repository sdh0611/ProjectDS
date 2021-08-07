// All rights reserve SDH (2021 ~ )


#include "DSInventoryEntity.h"
#include "DSCharacterBase.h"

// Sets default values
ADSInventoryEntity::ADSInventoryEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADSInventoryEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSInventoryEntity::GivenTo(ADSCharacterBase * NewOwner)
{
	OwnerCharacter = NewOwner;

}
