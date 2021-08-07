// All rights reserve SDH (2021 ~ )


#include "DSEquipment.h"
#include "Components/StaticMeshComponent.h"
#include "DSCharacterBase.h"

FName ADSEquipment::BodyMeshName = TEXT("BodyMesh");

ADSEquipment::ADSEquipment()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(BodyMeshName);
	if (BodyMesh)
	{
		SetRootComponent(BodyMesh);
	}

	bEquipped = false;
}

void ADSEquipment::BeginPlay()
{
	Super::BeginPlay();

}

void ADSEquipment::Equipped(ADSCharacterBase * EquipCharacter)
{
	if (IsValid(EquipCharacter))
	{
		bEquipped = true;
	}
}

void ADSEquipment::Unequipped()
{
	bEquipped = false;
}
