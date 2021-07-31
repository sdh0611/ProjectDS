// All rights reserve SDH (2021 ~ )

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDSFRAMEWORK_API UDSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDSCharacterMovementComponent();

public:
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	// Instead of GetMaxSpeedModifier(Deprecated from 4.3)
	virtual float GetMaxSpeed() const override;

	/** Returns maximum acceleration for the current state. */
	virtual float GetMaxAcceleration() const override;

	/** Get prediction data for a client game. Should not be used if not running as a client. Allocates the data on demand and can be overridden to allocate a custom override if desired. Result must be a FNetworkPredictionData_Client_Character. */
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	/** Get prediction data for a server game. Should not be used if not running as a server. Allocates the data on demand and can be overridden to allocate a custom override if desired. Result must be a FNetworkPredictionData_Server_Character. */
	virtual class FNetworkPredictionData_Server* GetPredictionData_Server() const override;

	void DoSprint(bool bSprint);
	void SetSprinting(bool bSprint);

	bool IsSprinting() const { return bSprinting; }
	bool WantsToSprint() const { return bWantsToSprint; }
	float GetCurrentSpeedMultiflier() const;

protected:
	/** Unpack compressed flags from a saved move and set state accordingly. See FSavedMove_Character. */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	/** Called after MovementMode has changed. Base implementation does special handling for starting certain modes, then notifies the CharacterOwner. */
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	/** Handle landing against Hit surface over remaingTime and iterations, calling SetPostLandedPhysics() and starting the new movement mode. */
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

private:
	UPROPERTY(EditAnywhere, Category = Move)
	float SprintSpeedMultiflier;
	
	UPROPERTY(Transient)
	uint8 bWantsToSprint : 1;

	UPROPERTY(Transient)
	uint8 bSprinting : 1;

	TWeakObjectPtr<class ADSCharacterBase> OwnerDSCharacter;
};

/** FSavedMove_Character represents a saved move on the client that has been sent to the server and might need to be played back. */
class FDSSavedMove_Character : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

public:
	FDSSavedMove_Character();
	virtual ~FDSSavedMove_Character();

public:
	/** Clear saved move properties, so it can be re-used. */
	virtual void Clear() override;

	/** Called to set up this saved move (when initially created) to make a predictive correction. */
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;

	/** Returns true if this move can be combined with NewMove for replication without changing any behavior */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

	/** Called before ClientUpdatePosition uses this SavedMove to make a predictive correction	 */
	virtual void PrepMoveFor(ACharacter* C) override;

	/** Returns a byte containing encoded special movement information (jumping, crouching, etc.)	 */
	/*
	 * CompressedFlags usage
	 * Sprint : FLAG_Custom_0
	 */
	virtual uint8 GetCompressedFlags() const override;

	uint32 bWantsToSprint : 1;

};

class FDSNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
	typedef FNetworkPredictionData_Client_Character Super;

public:
	FDSNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
	virtual ~FDSNetworkPredictionData_Client_Character();

	/** Allocate a new saved move. Subclasses should override this if they want to use a custom move class. */
	virtual FSavedMovePtr AllocateNewMove() override;

};