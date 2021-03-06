// SDH, All rights reserved. (2021 ~ )


#include "DSCharacterMovementComponent.h"
#include "DSCharacterBase.h"

UDSCharacterMovementComponent::UDSCharacterMovementComponent()
{
	SprintSpeedMultiflier = 1.5f;

	bWantsToSprint = false;
	bSprinting = false;

	bWantsToWalk = false;
	bWalking = false;
}

void UDSCharacterMovementComponent::SetUpdatedComponent(USceneComponent * NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	if (IsValid(CharacterOwner))
	{
		ADSCharacterBase* NewDSCharacter = Cast<ADSCharacterBase>(CharacterOwner);
		if (NewDSCharacter)
		{
			OwnerDSCharacter = NewDSCharacter;
		}
	}

}

float UDSCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	MaxSpeed *= GetCurrentSpeedMultiflier();

	return MaxSpeed;
}

float UDSCharacterMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();

	MaxAccel *= GetCurrentSpeedMultiflier();

	return MaxAccel;
}

FNetworkPredictionData_Client * UDSCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UDSCharacterMovementComponent* MutableThis = const_cast<UDSCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FDSNetworkPredictionData_Client_Character(*this);
	}

	return ClientPredictionData;
}

FNetworkPredictionData_Server * UDSCharacterMovementComponent::GetPredictionData_Server() const
{
	FNetworkPredictionData_Server* ServerPrediction = Super::GetPredictionData_Server();

	return ServerPrediction;
}

void UDSCharacterMovementComponent::DoSprint(bool bSprint)
{
	bWantsToSprint = bSprint;
	SetSprinting(bSprint);
}

void UDSCharacterMovementComponent::SetSprinting(bool bSprint)
{
	bSprinting = bSprint;
}

void UDSCharacterMovementComponent::DoWalk(bool bWalk)
{
	bWantsToWalk = bWalk;
	SetWalking(bWalk);
}

void UDSCharacterMovementComponent::SetWalking(bool bWalk)
{
	bWalking = bWalk;
}

float UDSCharacterMovementComponent::GetCurrentSpeedMultiflier() const
{
	float FinalSpeedMultiflier = 1.f;
	if (IsSprinting())
	{
		FinalSpeedMultiflier = SprintSpeedMultiflier;
	}
	else if (IsWalking())
	{
		FinalSpeedMultiflier = WalkSpeedMultiflier;
	}

	return FinalSpeedMultiflier;
}

void UDSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	if (OwnerDSCharacter.IsValid())
	{
		SetSprinting( (Flags & FDSSavedMove_Character::CompressedFlags::FLAG_Custom_0) != 0 );
		SetWalking( (Flags & FDSSavedMove_Character::CompressedFlags::FLAG_Custom_1) != 0 );
	}
}

void UDSCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	// DEPRECATED(21. 9.14) : Character class???? ?????????? ???? 
	//// ???? ???? ????
	//if (MovementMode == MOVE_Falling)
	//{
	//	bOrientRotationToMovement = false;
	//}
}

void UDSCharacterMovementComponent::ProcessLanded(const FHitResult & Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);

	// DEPRECATED(21. 9.14) : Character class???? ?????????? ???? 
	//bOrientRotationToMovement = true;
}

FDSSavedMove_Character::FDSSavedMove_Character()
{
	bWantsToSprint = false;
	bWantsToWalk = false;
}

FDSSavedMove_Character::~FDSSavedMove_Character()
{
}

void FDSSavedMove_Character::Clear()
{
	Super::Clear();

	bWantsToSprint = false;
	bWantsToWalk = false;
}

void FDSSavedMove_Character::SetMoveFor(ACharacter * C, float InDeltaTime, FVector const & NewAccel, FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (C && C->GetCharacterMovement())
	{
		UDSCharacterMovementComponent* DSMovement = Cast<UDSCharacterMovementComponent>(C->GetCharacterMovement());
		if (DSMovement)
		{
			bWantsToSprint = DSMovement->WantsToSprint();
			bWantsToWalk = DSMovement->WantsToWalk();
		}
	}

}

bool FDSSavedMove_Character::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * InCharacter, float MaxDelta) const
{
	if (NewMove)
	{
		const FDSSavedMove_Character* NewDSMove = static_cast<FDSSavedMove_Character*>(NewMove.Get());
		if (bWantsToSprint != NewDSMove->bWantsToSprint)
		{
			return false;
		}

		if (bWantsToWalk != NewDSMove->bWantsToWalk)
		{
			return false;
		}

		if (NewMove->bWasJumping || NewMove->bWantsToCrouch)
		{
			return false;
		}

		if (IsValid(InCharacter) && InCharacter->bIsCrouched)
		{
			return false;
		}
	}

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void FDSSavedMove_Character::PrepMoveFor(ACharacter * C)
{
	Super::PrepMoveFor(C);

	//if (IsValid(C) && C->GetCharacterMovement())
	//{
	//	UDSCharacterMovementComponent* DSMovement = Cast<UDSCharacterMovementComponent>(C->GetCharacterMovement());
	//	if (DSMovement)
	//	{
	//	}
	//}
}

uint8 FDSSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	// Combine sprint move
	if (bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	else if (bWantsToWalk)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

FDSNetworkPredictionData_Client_Character::FDSNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FDSNetworkPredictionData_Client_Character::~FDSNetworkPredictionData_Client_Character()
{
}

FSavedMovePtr FDSNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FDSSavedMove_Character());
}
