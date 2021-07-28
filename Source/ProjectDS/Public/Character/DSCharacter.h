// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSCharacterBase.h"
#include "DSCharacter.generated.h"

UCLASS()
class PROJECTDS_API ADSCharacter : public ADSCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
