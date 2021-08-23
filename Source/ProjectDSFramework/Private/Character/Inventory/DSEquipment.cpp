// All rights reserve SDH (2021 ~ )


#include "DSEquipment.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DSCharacterBase.h"
#include "Net/UnrealNetwork.h"

FName ADSEquipment::BodyMeshName = TEXT("BodyMesh");

ADSEquipment::ADSEquipment()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(BodyMeshName);
	if (BodyMesh)
	{
		SetRootComponent(BodyMesh);
	}

	bEquipped = false;
	bReplicates = true;
}

void ADSEquipment::BeginPlay()
{
	Super::BeginPlay();

}

void ADSEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ADSEquipment, bEquipped, COND_OwnerOnly);
	DOREPLIFETIME(ADSEquipment, bEquipped);
}

void ADSEquipment::PostNetReceive()
{
	Super::PostNetReceive();

}

void ADSEquipment::Equipped()
{
	if (!IsEquipped())
	{
		InternalEquipped();
		if (!IsNetMode(NM_DedicatedServer))
		{
			OnEquipped();
		}
	}
}

void ADSEquipment::Unequipped()
{
	if (IsEquipped())
	{
		InternalUnequipped();
		if (!IsNetMode(NM_DedicatedServer))
		{
			OnUnequipped();
		}
	}
}

void ADSEquipment::GivenTo(ADSCharacterBase * NewOwner)
{
	Super::GivenTo(NewOwner);

	Unequipped();
}

void ADSEquipment::InternalEquipped()
{
	bEquipped = true;
	SetActorHiddenInGame(false);
}

void ADSEquipment::InternalUnequipped()
{
	bEquipped = false;
	SetActorHiddenInGame(true);
}

void ADSEquipment::OnEquipped()
{
	AttachToCharacter(AttachSocketName);
}

void ADSEquipment::OnUnequipped()
{
	DetachFromCharacter();
}

void ADSEquipment::InternalDiscard()
{
	Super::InternalDiscard();

	Unequipped();
}

void ADSEquipment::AttachToCharacter(const FName & AttackSocketName)
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->GetMesh())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttackSocketName);
	}
}

void ADSEquipment::DetachFromCharacter()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ADSEquipment::OnRep_Equipped()
{
	if (bEquipped)
	{
		OnEquipped();
	}
	else
	{
		OnUnequipped();
	}
}
