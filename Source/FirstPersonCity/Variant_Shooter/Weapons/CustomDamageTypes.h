// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CustomDamageTypes.generated.h"

/**
 * Fire damage type for projectiles that deal fire damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UFireDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UFireDamageType();
};

/**
 * Ice damage type for projectiles that deal ice damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UIceDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UIceDamageType();
};

/**
 * Poison damage type for projectiles that deal poison damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UPoisonDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPoisonDamageType();
};

/**
 * Lightning damage type for projectiles that deal lightning damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API ULightningDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	ULightningDamageType();
};

/**
 * Explosive damage type for projectiles that deal explosive damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UExplosiveDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UExplosiveDamageType();
};