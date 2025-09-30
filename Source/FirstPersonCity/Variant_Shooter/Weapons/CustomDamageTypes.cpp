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

UJoyDamageType::UJoyDamageType()
{
	// Joy damage has uplifting effect with moderate impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 400.0f;
	DestructibleImpulse = 400.0f;
	DestructibleDamageSpreadScale = 1.5f;
	DamageFalloff = 1.0f;
}

UTrustDamageType::UTrustDamageType()
{
	// Trust damage has calming effect with low impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 150.0f;
	DestructibleImpulse = 150.0f;
	DestructibleDamageSpreadScale = 1.0f;
	DamageFalloff = 1.0f;
}

UTerrorDamageType::UTerrorDamageType()
{
	// Terror damage has frightening effect with high impulse
	bScaleMomentumByMass = false;
	DamageImpulse = 1000.0f;
	DestructibleImpulse = 1000.0f;
	DestructibleDamageSpreadScale = 3.0f;
	DamageFalloff = 1.5f;
}

UAmazingDamageType::UAmazingDamageType()
{
	// Amazing damage has stunning effect with moderate impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 600.0f;
	DestructibleImpulse = 600.0f;
	DestructibleDamageSpreadScale = 2.0f;
	DamageFalloff = 1.0f;
}

USadnessDamageType::USadnessDamageType()
{
	// Sadness damage has depressing effect with low impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 100.0f;
	DestructibleImpulse = 100.0f;
	DestructibleDamageSpreadScale = 0.8f;
	DamageFalloff = 1.0f;
}

ULoathingDamageType::ULoathingDamageType()
{
	// Loathing damage has disgusting effect with moderate impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 500.0f;
	DestructibleImpulse = 500.0f;
	DestructibleDamageSpreadScale = 1.8f;
	DamageFalloff = 1.2f;
}

URageDamageType::URageDamageType()
{
	// Rage damage has violent effect with very high impulse
	bScaleMomentumByMass = false;
	DamageImpulse = 1300.0f;
	DestructibleImpulse = 1300.0f;
	DestructibleDamageSpreadScale = 4.0f;
	DamageFalloff = 1.8f;
}

UExpectDamageType::UExpectDamageType()
{
	// Expect damage has anticipatory effect with variable impulse
	bScaleMomentumByMass = true;
	DamageImpulse = 300.0f;
	DestructibleImpulse = 300.0f;
	DestructibleDamageSpreadScale = 1.2f;
	DamageFalloff = 1.0f;
}