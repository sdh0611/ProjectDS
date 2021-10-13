// SDH, All rights reserved. (2021 ~ )


#include "DSCharacterStatComponent.h"
#include "DSCharacterBase.h"

// Sets default values for this component's properties
UDSCharacterStatComponent::UDSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	Attack = 10.f;
	Defence = 10.f;
}


// Called when the game starts
void UDSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	ADSCharacterBase* NewOwner = GetOwner<ADSCharacterBase>();
	if (IsValid(NewOwner))
	{
		CachedOwnerPrivate = NewOwner;
	}
}

void UDSCharacterStatComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = FMath::Max(0.f, NewMaxHealth);
}

void UDSCharacterStatComponent::SetCurrentHealth(float NewCurrentHealth)
{
	CurrentHealth = FMath::Clamp<float>(NewCurrentHealth, 0.f, MaxHealth);
}

void UDSCharacterStatComponent::SetAttack(float NewAttack)
{
	Attack = FMath::Max(0.f, NewAttack);
}

void UDSCharacterStatComponent::SetDefence(float NewDefence)
{
	Defence = FMath::Max(0.f, NewDefence);
}