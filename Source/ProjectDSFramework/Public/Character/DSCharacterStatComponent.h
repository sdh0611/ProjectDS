// SDH, All rights reserved. (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSCharacterStatComponent.generated.h"

/*
 * Character의 스탯 설정 및 접근을 위한 컴포넌트.
 * 스탯 관리 이상의 일을 시키진 말 것.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDSFRAMEWORK_API UDSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDSCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// ~ Begin UActorComponent Inferface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ~ End UActorComponent Inferface

public:
	// ~ Begin UDSCharacterStatComponent Interface
	void TakeDamage(float Damage, FDamageEvent const & DamageEvent);
	void SetMaxHealth(float NewMaxHealth);
	void SetCurrentHealth(float NewCurrentHealth);
	void SetAttack(float NewAttack);
	void SetDefence(float NewDefence);

	float GetMaxHealth() const { return MaxHealth; }
	float GetCurrentHealth() const { return CurrentHealth; }
	float GetAttack() const { return Attack; }
	float GetDefence() const { return Defence; }
	// ~ End UDSCharacterStatComponent Interface

private:
	UPROPERTY(EditAnywhere, Meta = (AllowAccessPrivate = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, Meta = (AllowAccessPrivate = true), Replicated)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Meta = (AllowAccessPrivate = true))
	float Attack;

	UPROPERTY(EditAnywhere, Meta = (AllowAccessPrivate = true))
	float Defence;

private:
	TWeakObjectPtr<class ADSCharacterBase> CachedOwnerPrivate;

};
