// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/DataTable.h"
#include "CustomDamageTypes.h"

AShooterProjectile::AShooterProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the collision component and assign it as the root
	RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));

	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	// create the projectile movement component. No need to attach it because it's not a Scene Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bShouldBounce = true;

	// set the default damage type
	HitDamageType = UDamageType::StaticClass();
}

void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// ignore the pawn that shot this projectile
	CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	// Initialize with data table if specified
	if (ProjectileDataHandle.DataTable && !ProjectileDataHandle.RowName.IsNone())
	{
		InitializeWithDataTableRow(ProjectileDataHandle);
	}
}

void AShooterProjectile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the destruction timer
	GetWorld()->GetTimerManager().ClearTimer(DestructionTimer);
}

void AShooterProjectile::InitializeWithData(const FProjectileData& Data)
{
	ApplyProjectileData(Data);
}

void AShooterProjectile::InitializeWithDataTableRow(const FDataTableRowHandle& RowHandle)
{
	if (RowHandle.DataTable)
	{
		if (FProjectileData* ProjectileData = RowHandle.DataTable->FindRow<FProjectileData>(RowHandle.RowName, TEXT("Projectile Data Lookup")))
		{
			ApplyProjectileData(*ProjectileData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find projectile data row: %s"), *RowHandle.RowName.ToString());
		}
	}
}

FProjectileData AShooterProjectile::GetProjectileData() const
{
	FProjectileData CurrentData;
	CurrentData.HitDamage = HitDamage;
	CurrentData.HitDamageType = HitDamageType;
	CurrentData.PhysicsForce = PhysicsForce;
	CurrentData.InitialSpeed = ProjectileMovement->InitialSpeed;
	CurrentData.MaxSpeed = ProjectileMovement->MaxSpeed;
	CurrentData.bShouldBounce = ProjectileMovement->bShouldBounce;
	CurrentData.bExplodeOnHit = bExplodeOnHit;
	CurrentData.ExplosionRadius = ExplosionRadius;
	CurrentData.CollisionRadius = CollisionComponent->GetScaledSphereRadius();
	CurrentData.DeferredDestructionTime = DeferredDestructionTime;
	CurrentData.NoiseLoudness = NoiseLoudness;
	CurrentData.NoiseRange = NoiseRange;
	CurrentData.NoiseTag = NoiseTag;
	CurrentData.bDamageOwner = bDamageOwner;
	return CurrentData;
}

void AShooterProjectile::ApplyProjectileData(const FProjectileData& Data)
{
	// Apply damage properties
	HitDamage = Data.HitDamage;
	HitDamageType = Data.HitDamageType;
	PhysicsForce = Data.PhysicsForce;
	bDamageOwner = Data.bDamageOwner;
	
	// Apply explosion properties
	bExplodeOnHit = Data.bExplodeOnHit;
	ExplosionRadius = Data.ExplosionRadius;
	
	// Apply lifetime properties
	DeferredDestructionTime = Data.DeferredDestructionTime;
	
	// Apply noise properties
	NoiseLoudness = Data.NoiseLoudness;
	NoiseRange = Data.NoiseRange;
	NoiseTag = Data.NoiseTag;
	
	// Apply collision properties
	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(Data.CollisionRadius);
	}
	
	// Apply movement properties
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = Data.InitialSpeed;
		ProjectileMovement->MaxSpeed = Data.MaxSpeed;
		ProjectileMovement->bShouldBounce = Data.bShouldBounce;
	}

	// Log the configuration for debugging
	UE_LOG(LogTemp, Log, TEXT("Projectile configured: %s, Damage: %.1f, Type: %s"), 
		*Data.ProjectileName, 
		Data.HitDamage, 
		Data.HitDamageType ? *Data.HitDamageType->GetName() : TEXT("None"));
}

void AShooterProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// ignore if we've already hit something else
	if (bHit)
	{
		return;
	}

	bHit = true;

	// disable collision on the projectile
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// make AI perception noise
	MakeNoise(NoiseLoudness, GetInstigator(), GetActorLocation(), NoiseRange, NoiseTag);

	if (bExplodeOnHit)
	{
		
		// apply explosion damage centered on the projectile
		ExplosionCheck(GetActorLocation());

	} else {

		// single hit projectile. Process the collided actor
		ProcessHit(Other, OtherComp, Hit.ImpactPoint, -Hit.ImpactNormal);

	}

	// pass control to BP for any extra effects
	BP_OnProjectileHit(Hit);

	// check if we should schedule deferred destruction of the projectile
	if (DeferredDestructionTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &AShooterProjectile::OnDeferredDestruction, DeferredDestructionTime, false);

	} else {

		// destroy the projectile right away
		Destroy();
	}
}

void AShooterProjectile::ExplosionCheck(const FVector& ExplosionCenter)
{
	// do a sphere overlap check look for nearby actors to damage
	TArray<FOverlapResult> Overlaps;

	FCollisionShape OverlapShape;
	OverlapShape.SetSphere(ExplosionRadius);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (!bDamageOwner)
	{
		QueryParams.AddIgnoredActor(GetInstigator());
	}

	GetWorld()->OverlapMultiByObjectType(Overlaps, ExplosionCenter, FQuat::Identity, ObjectParams, OverlapShape, QueryParams);

	TArray<AActor*> DamagedActors;

	// process the overlap results
	for (const FOverlapResult& CurrentOverlap : Overlaps)
	{
		// overlaps may return the same actor multiple times per each component overlapped
		// ensure we only damage each actor once by adding it to a damaged list
		if (DamagedActors.Find(CurrentOverlap.GetActor()) == INDEX_NONE)
		{
			DamagedActors.Add(CurrentOverlap.GetActor());

			// apply physics force away from the explosion
			const FVector& ExplosionDir = CurrentOverlap.GetActor()->GetActorLocation() - GetActorLocation();

			// push and/or damage the overlapped actor
			ProcessHit(CurrentOverlap.GetActor(), CurrentOverlap.GetComponent(), GetActorLocation(), ExplosionDir.GetSafeNormal());
		}
			
	}
}

void AShooterProjectile::ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection)
{
	// have we hit a character?
	if (ACharacter* HitCharacter = Cast<ACharacter>(HitActor))
	{
		// ignore the owner of this projectile
		if (HitCharacter != GetOwner() || bDamageOwner)
		{
			// apply damage to the character with the configured damage type
			UGameplayStatics::ApplyDamage(HitCharacter, HitDamage, GetInstigator()->GetController(), this, HitDamageType);
		}
	}

	// have we hit a physics object?
	if (HitComp->IsSimulatingPhysics())
	{
		// give some physics impulse to the object
		HitComp->AddImpulseAtLocation(HitDirection * PhysicsForce, HitLocation);
	}
}

void AShooterProjectile::OnDeferredDestruction()
{
	// destroy this actor
	Destroy();
}
