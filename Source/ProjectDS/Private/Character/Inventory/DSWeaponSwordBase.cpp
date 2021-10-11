// SDH, All rights reserved. (2021 ~ )


#include "DSWeaponSwordBase.h"
#include "DSCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "..\..\..\Public\Character\Inventory\DSWeaponSwordBase.h"


ADSWeaponSwordBase::ADSWeaponSwordBase()
{
}

void ADSWeaponSwordBase::BeginPlay()
{
	Super::BeginPlay();

	AttackHitCheckHelper.Initialize(this);
}

void ADSWeaponSwordBase::InternalUpdateWeapon(float DeltaTime)
{
	Super::InternalUpdateWeapon(DeltaTime);

	UpdateAttackSequence(DeltaTime);

	if (HasAuthority())
	{
		AttackHitCheckHelper.Update(DeltaTime);
	}
}

void ADSWeaponSwordBase::UpdateAttackSequence(float DeltaTime)
{
	FDSWeaponAttackSequence* CurSequence = GetAttackSequence(CurrentCombo);
	if (CurSequence && CurSequence->IsAttacking())
	{
		if (CurSequence->IsSequenceEnd())
		{
			CurSequence->Reset();
			//CurrentCombo = 0;
			CurrentCombo = INDEX_NONE;
			if (OwnerCharacter.IsValid())
			{
				OwnerCharacter->OnAttackEnd();
			}

			PendingAttack.Empty();
		}
		else
		{
			CurSequence->Update(DeltaTime);
		}
	}
}

bool ADSWeaponSwordBase::CanAttack() const
{
	const bool bCanAttack = Super::CanAttack() && IsWeaponArmed();

	return bCanAttack;
}

//bool ADSWeaponSwordBase::DoAttack()
//{
//	if (OwnerCharacter.IsValid())
//	{
//		const int32 NextCombo = CurrentCombo + 1;
//		FDSWeaponAttackSequence* Sequence = GetAttackSequence(NextCombo);
//		if (Sequence)
//		{
//			bool bCanAttack = false;
//			if (!Sequence->IsAttacking())
//			{
//				bCanAttack = true;
//			}
//			else if (Sequence->CanCombo() && AttackSequence.IsValidIndex(NextCombo))
//			{
//				// Reset prev attack sequence
//				FDSWeaponAttackSequence* PrevSequence = GetAttackSequence(CurrentCombo);
//				if (PrevSequence)
//				{
//					Sequence->Reset();
//				}
//
//				// Get next attack sequence
//				Sequence = GetAttackSequence(NextCombo);
//				if (Sequence)
//				{
//					bCanAttack = true;
//				}
//			}
//
//			if (bCanAttack)
//			{
//				CurrentCombo = NextCombo;
//
//				Sequence->Attack();
//				DisableCharacterMovementInput();
//				//OwnerCharacter->SetCharacterInputFlag(ADSCharacterBase::EActiveInputFlag::InputAll);
//				//OwnerCharacter->DisableCharacterInput(ADSCharacterBase::EActiveInputFlag::InputJump | ADSCharacterBase::EActiveInputFlag::InputEquipWeapon);
//				//GetWorldTimerManager().SetTimer(MoveInputTimer, this, &ADSWeaponSwordBase::DisableCharacterMovement, Sequence->GetMoveAllowTimeInterval(), false);
//				if (Sequence->AttackAnim.WeaponAnim)
//				{
//					OwnerCharacter->PlayAnimMontage(Sequence->AttackAnim.WeaponAnim, Sequence->AttackAnim.PlayRate);
//				}
//			}
//
//			return bCanAttack;
//		}
//	}
//
//	return false;
//}

bool ADSWeaponSwordBase::DoAttack()
{
	if (OwnerCharacter.IsValid())
	{
		bool bCanAttack = false;
		const int32 NextCombo = CurrentCombo + 1;
		FDSWeaponAttackSequence* Sequence = GetAttackSequence(CurrentCombo);
		FDSWeaponAttackSequence* NextSequence = GetAttackSequence(NextCombo);

		if (nullptr == Sequence)
		{
			// First attack
			if (NextSequence)
			{
				bCanAttack = true;
			}
		}
		else
		{
			if (Sequence->CanCombo() && NextSequence)
			{
				// Reset current attack sequence
				Sequence->Reset();
				bCanAttack = true;
			}
		}

		if (bCanAttack)
		{
			CurrentCombo = NextCombo;
			
			// Check hit check in process
			if (AttackHitCheckHelper.IsInHitCheckProcess())
			{
				AttackHitCheckHelper.Reset();
			}

			// Update attack sequence
			Sequence = NextSequence;
			Sequence->Attack();
			if (Sequence->AttackAnim.WeaponAnim)
			{
				OwnerCharacter->PlayAnimMontage(Sequence->AttackAnim.WeaponAnim, Sequence->AttackAnim.PlayRate);
			}

			// Character
			OwnerCharacter->OnAttackStart();
		}

		return bCanAttack;
	}

	return false;
}

const FDSWeaponAttackSequence* ADSWeaponSwordBase::GetAttackSequence(int32 Index) const
{
	if (AttackSequence.IsValidIndex(Index))
	{
		return &AttackSequence[Index];
	}

	return nullptr;
}

FDSWeaponAttackSequence* ADSWeaponSwordBase::GetAttackSequence(int32 Index)
{
	if (AttackSequence.IsValidIndex(Index))
	{
		return &AttackSequence[Index];
	}

	return nullptr;
}


void ADSWeaponSwordBase::TryAttack(uint8 TryAttackType)
{
	Super::TryAttack(TryAttackType);
}

void ADSWeaponSwordBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ADSWeaponSwordBase::PostNetReceive()
{
	Super::PostNetReceive();

}

void ADSWeaponSwordBase::RequestHitCheckStart()
{
	if (HasAuthority())
	{
		UE_LOG(LogClass, Warning, TEXT("[WeaponDebugLog] Request hit check start."));
		AttackHitCheckHelper.HitCheckStart();
	}
}

void ADSWeaponSwordBase::RequestHitCheckEnd()
{
	if (HasAuthority())
	{
		UE_LOG(LogClass, Warning, TEXT("[WeaponDebugLog] Request hit check end."));
		AttackHitCheckHelper.HitCheckEnd();
	}
}

void ADSWeaponSwordBase::InternalUnequipped()
{
	Super::InternalUnequipped();

	AttackHitCheckHelper.Reset();
}

void FDSAttackHitCheckHelper::Initialize(ADSWeaponSwordBase * OwnerSwordWeapon)
{
	OwnerPrivate = OwnerSwordWeapon;

	if (OwnerPrivate.IsValid())
	{
		CollisionQuery.bReturnPhysicalMaterial = false;
		CollisionQuery.bTraceComplex = false;
	}
	AttackTraceFootstep.AddZeroed(HitCheckTraceNum);
}

void FDSAttackHitCheckHelper::Reset()
{
	ElapsedTime = 0.f;
	bActive = false;

	CollisionQuery.ClearIgnoredActors();
	
	UE_LOG(LogClass, Warning, TEXT("[HitCheckDebugLog] Reset Helper."));

	if (AttackTraceFootstep.Num() == HitCheckTraceNum)
	{
		for (int32 Index = 0; Index < HitCheckTraceNum; ++Index)
		{
			AttackTraceFootstep[Index] = FVector::ZeroVector;
		}
	}
}

void FDSAttackHitCheckHelper::Update(float DeltaTime)
{
	if (IsInHitCheckProcess())
	{
		if (ElapsedTime <= HitCheckInterval)
		{
			ElapsedTime += DeltaTime;
		}
		else
		{
			// Do hit check
			HitCheck();
			ElapsedTime = 0.f;
		}
	}
}

void FDSAttackHitCheckHelper::HitCheckStart()
{
	// Prepare new attack sequence hit check
	//UE_LOG(LogClass, Warning, TEXT("[HitCheckDebugLog] AttackCheckStart"));

	// Add ignored actor
	if (OwnerPrivate.IsValid())
	{
		CollisionQuery.AddIgnoredActor(OwnerPrivate->GetInventoryEntityOwner());
	}

	// Initialize trace locaiton
	UpdateAttackTraceFootstep();

	// Set active
	bActive = true;
}

void FDSAttackHitCheckHelper::HitCheckEnd()
{
	//UE_LOG(LogClass, Warning, TEXT("[HitCheckDebugLog] AttackCheckEnd"));
	Reset();
}

void FDSAttackHitCheckHelper::UpdateAttackTraceFootstep()
{
	check(AttackTraceFootstep.Num() == HitCheckTraceNum);

	// Initialize trace locaiton
	if (OwnerPrivate.IsValid())
	{
		const int32 FootstepCount = AttackTraceFootstep.Num();
		if (FootstepCount > 0)
		{
			AttackTraceFootstep[0] = OwnerPrivate->GetSocketLocation(WeaponTraceStart);
			AttackTraceFootstep[FootstepCount - 1] = OwnerPrivate->GetSocketLocation(WeaponTraceEnd);

			FVector DirNormalized(AttackTraceFootstep.Last() - AttackTraceFootstep[0]);
			const float Length = DirNormalized.Size() / (HitCheckTraceNum - 1);
			if (DirNormalized.Normalize())
			{
				DirNormalized *= Length;  
				for (int32 Index = 1; Index < FootstepCount - 1; ++Index)
				{
					AttackTraceFootstep[Index] = AttackTraceFootstep[Index - 1] + DirNormalized;
				}
			}
		}
	}

}

void FDSAttackHitCheckHelper::HitCheck()
{
	if(OwnerPrivate.IsValid())
	{
		UWorld* World = OwnerPrivate->GetWorld();
		bool bHit = false;
		if (World)
		{
			// Cache last trace location
			const TArray<FVector> PrevAttackTraceFootstep = AttackTraceFootstep;
			// Update current trace location
			UpdateAttackTraceFootstep();

			TArray<FHitResult> Hits;
			for (int32 Index = 0; Index < HitCheckTraceNum; ++Index)
			{
				World->LineTraceMultiByChannel(Hits, PrevAttackTraceFootstep[Index], AttackTraceFootstep[Index], ECollisionChannel::ECC_GameTraceChannel1, CollisionQuery);
				//UE_LOG(LogClass, Warning, TEXT("[HitCheckDebugLog] PrevFootStep : %s, CurFootStep : %s"), *PrevAttackTraceFootstep[Index].ToString(), *AttackTraceFootstep[Index].ToString());
				if (Hits.Num() > 0)
				{
					AController* const AttackInstigator = OwnerPrivate->GetInventoryEntityOwner() ? OwnerPrivate->GetInventoryEntityOwner()->GetController() : nullptr;
					for (FHitResult& Hit : Hits)
					{
						if (Hit.Actor.IsValid())
						{
							//UE_LOG(LogClass, Warning, TEXT("[HitCheckDebugLog|%f] Hit actor : %s, Hit component : %s !!!"), World->GetTimeSeconds(), *(Hit.Actor->GetName()), *(Hit.Component.IsValid() ? Hit.Component->GetName() : FString()));
							Hit.Actor->TakeDamage(30.f, FPointDamageEvent(), AttackInstigator, OwnerPrivate.Get());
							// Add hit actor to ignored
							CollisionQuery.AddIgnoredActor(Hit.Actor.Get());
							if (!bHit)
							{
								bHit = true;
							}
						}
					}
				}

				//DrawDebugLine(World, PrevAttackTraceFootstep[Index], AttackTraceFootstep[Index], FColor::Red, true, 5.f);
			}
		}

		if (bHit)
		{
			// Test code
			if (OwnerPrivate->GetInventoryEntityOwner())
			{
				OwnerPrivate->GetInventoryEntityOwner()->OnAttackHit();
			}
		}
	}

}
