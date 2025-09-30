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

/**
 * Joy damage type for projectiles that deal joy-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UJoyDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UJoyDamageType();
};

/**
 * Trust damage type for projectiles that deal trust-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UTrustDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UTrustDamageType();
};

/**
 * Terror damage type for projectiles that deal terror-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UTerrorDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UTerrorDamageType();
};

/**
 * Amazing damage type for projectiles that deal amazement-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UAmazingDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UAmazingDamageType();
};

/**
 * Sadness damage type for projectiles that deal sadness-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API USadnessDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	USadnessDamageType();
};

/**
 * Loathing damage type for projectiles that deal loathing-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API ULoathingDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	ULoathingDamageType();
};

/**
 * Rage damage type for projectiles that deal rage-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API URageDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	URageDamageType();
};

/**
 * Expect damage type for projectiles that deal expectation-based emotional damage
 */
UCLASS(BlueprintType)
class FIRSTPERSONCITY_API UExpectDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UExpectDamageType();
};