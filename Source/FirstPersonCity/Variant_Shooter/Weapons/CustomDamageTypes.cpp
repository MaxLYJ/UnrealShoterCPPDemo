// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomDamageTypes.h"

UFireDamageType::UFireDamageType()
{
	// Fire damage doesn't scale with mass and has higher impulse
	bScaleMomentumByMass = false;
	DamageImpulse = 800.0f;
	DestructibleImpulse = 800.0f;
	DestructibleDamageSpreadScale = 3.0f;
	DamageFalloff = 1.0f;
}

UIceDamageType::UIceDamageType()
{
	// Ice damage has reduced impulse (freezing effect)
	bScaleMomentumByMass = true;
	DamageImpulse = 200.0f;
	DestructibleImpulse = 200.0f;
	DestructibleDamageSpreadScale = 1.0f;
	DamageFalloff = 1.0f;
}

UPoisonDamageType::UPoisonDamageType()
{
	// Poison damage has minimal impulse (damage over time effect)
	bScaleMomentumByMass = true;
	DamageImpulse = 50.0f;
	DestructibleImpulse = 50.0f;
	DestructibleDamageSpreadScale = 0.5f;
	DamageFalloff = 1.0f;
}

ULightningDamageType::ULightningDamageType()
{
	// Lightning damage has high impulse and doesn't scale with mass
	bScaleMomentumByMass = false;
	DamageImpulse = 1200.0f;
	DestructibleImpulse = 1200.0f;
	DestructibleDamageSpreadScale = 2.0f;
	DamageFalloff = 1.0f;
}

UExplosiveDamageType::UExplosiveDamageType()
{
	// Explosive damage has very high impulse and spreads well
	bScaleMomentumByMass = false;
	bRadialDamageVelChange = true;
	DamageImpulse = 1500.0f;
	DestructibleImpulse = 1500.0f;
	DestructibleDamageSpreadScale = 5.0f;
	DamageFalloff = 2.0f; // Quadratic falloff for explosions
}