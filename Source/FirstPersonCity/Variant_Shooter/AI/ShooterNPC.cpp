// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/AI/ShooterNPC.h"
#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "ShooterGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"

void AShooterNPC::BeginPlay()
{
	Super::BeginPlay();

	// spawn the weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);
}

void AShooterNPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the death timer
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

float AShooterNPC::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ignore if already dead
	if (bIsDead)
	{
		return 0.0f;
	}

	// Extract damage type information
	FString DamageTypeName = TEXT("Unknown");
	FString DamageEventType = TEXT("Generic");
	FString AdditionalInfo = TEXT("");

	if (DamageEvent.DamageTypeClass)
	{
		// Get the damage type class and extract its name
		UClass* DamageTypeClass = DamageEvent.DamageTypeClass;
		if (DamageTypeClass)
		{
			DamageTypeName = DamageTypeClass->GetName();
		}
	}

	// Determine the specific damage event type and extract additional information
	int32 EventTypeID = DamageEvent.GetTypeID();
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		DamageEventType = TEXT("Point Damage");
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (PointDamageEvent)
		{
			AdditionalInfo = FString::Printf(TEXT("Hit Location: %s, Shot Direction: %s"), 
				*PointDamageEvent->HitInfo.Location.ToString(),
				*PointDamageEvent->ShotDirection.ToString());
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		DamageEventType = TEXT("Radial Damage");
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
		if (RadialDamageEvent)
		{
			AdditionalInfo = FString::Printf(TEXT("Origin: %s, Radius: %.1f"), 
				*RadialDamageEvent->Origin.ToString(),
				RadialDamageEvent->Params.OuterRadius);
		}
	}
	else
	{
		DamageEventType = TEXT("Generic Damage");
	}

	// Get damage causer information
	FString DamageCauserName = DamageCauser ? DamageCauser->GetName() : TEXT("Unknown");
	FString InstigatorName = TEXT("Unknown");
	if (EventInstigator)
	{
		APawn* InstigatorPawn = EventInstigator->GetPawn();
		InstigatorName = InstigatorPawn ? InstigatorPawn->GetName() : EventInstigator->GetName();
	}

	// Create comprehensive damage message
	FString DamageMessage = FString::Printf(TEXT("%s received %.1f %s damage of type: %s"), 
		*GetName(), Damage, *DamageEventType, *DamageTypeName);
	
	FString CauserMessage = FString::Printf(TEXT("Caused by: %s (Instigator: %s)"), 
		*DamageCauserName, *InstigatorName);

	// Print to screen (visible for 5 seconds)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DamageMessage);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, CauserMessage);
		
		if (!AdditionalInfo.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, AdditionalInfo);
		}
	}

	// Also log to console with more details
	UE_LOG(LogTemp, Warning, TEXT("=== DAMAGE RECEIVED ==="));
	UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("Damage Amount: %.1f"), Damage);
	UE_LOG(LogTemp, Warning, TEXT("Damage Type: %s"), *DamageTypeName);
	UE_LOG(LogTemp, Warning, TEXT("Event Type: %s"), *DamageEventType);
	UE_LOG(LogTemp, Warning, TEXT("Damage Causer: %s"), *DamageCauserName);
	UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *InstigatorName);
	UE_LOG(LogTemp, Warning, TEXT("HP Before: %.1f, HP After: %.1f"), CurrentHP, CurrentHP - Damage);
	
	if (!AdditionalInfo.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Additional Info: %s"), *AdditionalInfo);
	}
	UE_LOG(LogTemp, Warning, TEXT("======================="));

	// Reduce HP
	CurrentHP -= Damage;

	// Have we depleted HP?
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AShooterNPC::AttachWeaponMeshes(AShooterWeapon* WeaponToAttach)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// attach the weapon actor
	WeaponToAttach->AttachToActor(this, AttachmentRule);

	// attach the weapon meshes
	WeaponToAttach->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	WeaponToAttach->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
}

void AShooterNPC::PlayFiringMontage(UAnimMontage* Montage)
{
	// unused
}

void AShooterNPC::AddWeaponRecoil(float Recoil)
{
	// unused
}

void AShooterNPC::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	// unused
}

FVector AShooterNPC::GetWeaponTargetLocation()
{
	// start aiming from the camera location
	const FVector AimSource = GetFirstPersonCameraComponent()->GetComponentLocation();

	FVector AimDir, AimTarget = FVector::ZeroVector;

	// do we have an aim target?
	if (CurrentAimTarget)
	{
		// target the actor location
		AimTarget = CurrentAimTarget->GetActorLocation();

		// apply a vertical offset to target head/feet
		AimTarget.Z += FMath::RandRange(MinAimOffsetZ, MaxAimOffsetZ);

		// get the aim direction and apply randomness in a cone
		AimDir = (AimTarget - AimSource).GetSafeNormal();
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDir, AimVarianceHalfAngle);

		
	} else {

		// no aim target, so just use the camera facing
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetFirstPersonCameraComponent()->GetForwardVector(), AimVarianceHalfAngle);

	}

	// calculate the unobstructed aim target location
	AimTarget = AimSource + (AimDir * AimRange);

	// run a visibility trace to see if there's obstructions
	FHitResult OutHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, AimSource, AimTarget, ECC_Visibility, QueryParams);

	// return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterNPC::AddWeaponClass(const TSubclassOf<AShooterWeapon>& InWeaponClass)
{
	// unused
}

void AShooterNPC::OnWeaponActivated(AShooterWeapon* InWeapon)
{
	// unused
}

void AShooterNPC::OnWeaponDeactivated(AShooterWeapon* InWeapon)
{
	// unused
}

void AShooterNPC::OnSemiWeaponRefire()
{
	// are we still shooting?
	if (bIsShooting)
	{
		// fire the weapon
		Weapon->StartFiring();
	}
}

void AShooterNPC::Die()
{
	// ignore if already dead
	if (bIsDead)
	{
		return;
	}

	// raise the dead flag
	bIsDead = true;

	// increment the team score
	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}

	// disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// stop movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	// enable ragdoll physics on the third person mesh
	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.0f);

	// schedule actor destruction
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AShooterNPC::DeferredDestruction, DeferredDestructionTime, false);
}

void AShooterNPC::DeferredDestruction()
{
	Destroy();
}

void AShooterNPC::StartShooting(AActor* ActorToShoot)
{
	// save the aim target
	CurrentAimTarget = ActorToShoot;

	// raise the flag
	bIsShooting = true;

	// signal the weapon
	Weapon->StartFiring();
}

void AShooterNPC::StopShooting()
{
	// lower the flag
	bIsShooting = false;

	// signal the weapon
	Weapon->StopFiring();
}

void AShooterNPC::TestDamageTypes()
{
	if (bIsDead)
	{
		return;
	}

	// Test different damage types
	FString TestMessage = FString::Printf(TEXT("Testing damage types on %s"), *GetName());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TestMessage);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TestMessage);

	// Reset HP for testing
	CurrentHP = 100.0f;

	// Test basic damage
	FDamageEvent BasicDamage(UDamageType::StaticClass());
	TakeDamage(10.0f, BasicDamage, nullptr, this);

	// Test point damage
	FHitResult HitResult;
	HitResult.Location = GetActorLocation();
	HitResult.ImpactPoint = GetActorLocation();
	FPointDamageEvent PointDamage(15.0f, HitResult, FVector(1, 0, 0), UDamageType::StaticClass());
	TakeDamage(15.0f, PointDamage, nullptr, this);

	// Test radial damage
	FRadialDamageEvent RadialDamage;
	RadialDamage.DamageTypeClass = UDamageType::StaticClass();
	RadialDamage.Origin = GetActorLocation() + FVector(0, 0, 100);
	RadialDamage.Params.BaseDamage = 20.0f;
	RadialDamage.Params.OuterRadius = 500.0f;
	TakeDamage(20.0f, RadialDamage, nullptr, this);

	UE_LOG(LogTemp, Warning, TEXT("Damage type testing completed for %s"), *GetName());
}
