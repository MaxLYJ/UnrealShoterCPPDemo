// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/DamageType.h"
#include "ProjectileData.generated.h"

/**
 * Data structure for projectile configuration that can be used in Data Tables
 * This allows designers to create multiple projectile types without code changes
 */
USTRUCT(BlueprintType)
struct FIRSTPERSONCITY_API FProjectileData : public FTableRowBase
{
	GENERATED_BODY()

	FProjectileData()
	{
		ProjectileName = TEXT("Standard Projectile");
		HitDamage = 25.0f;
		HitDamageType = UDamageType::StaticClass();
		PhysicsForce = 100.0f;
		InitialSpeed = 3000.0f;
		MaxSpeed = 3000.0f;
		bShouldBounce = true;
		bExplodeOnHit = false;
		ExplosionRadius = 500.0f;
		CollisionRadius = 16.0f;
		DeferredDestructionTime = 5.0f;
		NoiseLoudness = 3.0f;
		NoiseRange = 3000.0f;
		NoiseTag = FName("Projectile");
		bDamageOwner = false;
	}

	/** Display name for this projectile type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FString ProjectileName;

	/** Damage to apply on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = 0, ClampMax = 1000))
	float HitDamage;

	/** Type of damage to apply (Fire, Ice, Poison, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> HitDamageType;

	/** Physics force to apply on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = 0, ClampMax = 50000))
	float PhysicsForce;

	/** Initial speed of the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0, ClampMax = 20000, Units = "cm/s"))
	float InitialSpeed;

	/** Max speed of the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0, ClampMax = 20000, Units = "cm/s"))
	float MaxSpeed;

	/** Whether the projectile should bounce */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShouldBounce;

	/** If true, the projectile will explode on hit and cause area damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool bExplodeOnHit;

	/** Explosion radius for area damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion", meta = (ClampMin = 0, ClampMax = 5000, Units = "cm"))
	float ExplosionRadius;

	/** Projectile collision radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (ClampMin = 1, ClampMax = 100, Units = "cm"))
	float CollisionRadius;

	/** Time before projectile self-destructs after hitting something */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifetime", meta = (ClampMin = 0, ClampMax = 30, Units = "s"))
	float DeferredDestructionTime;

	/** AI perception noise loudness */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception", meta = (ClampMin = 0, ClampMax = 100))
	float NoiseLoudness;

	/** AI perception noise range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float NoiseRange;

	/** AI perception noise tag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FName NoiseTag;

	/** If true, the projectile can damage the character that shot it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bDamageOwner;

	/** Optional description for this projectile type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General", meta = (MultiLine = true))
	FString Description;
};