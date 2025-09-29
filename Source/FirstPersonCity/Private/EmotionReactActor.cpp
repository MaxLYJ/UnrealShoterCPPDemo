// Fill out your copyright notice in the Description page of Project Settings.

#include "EmotionReactActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"
#include "Variant_Shooter/Weapons/CustomDamageTypes.h"

// Sets default values
AEmotionReactActor::AEmotionReactActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AEmotionReactActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEmotionReactActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Implementation of TakeDamage function
float AEmotionReactActor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call parent implementation first
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Log the damage received
	UE_LOG(LogTemp, Warning, TEXT("%s received %.1f damage"), *GetName(), ActualDamage);

	// Display on screen for debugging
	if (GEngine)
	{
		FString DamageMessage = FString::Printf(TEXT("%s took %.1f damage!"), *GetName(), ActualDamage);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, DamageMessage);
	}

	// Check damage type and apply visual effects
	if (DamageEvent.DamageTypeClass)
	{

	BP_OnProjectileHit(DamageEvent.DamageTypeClass);

		if (DamageEvent.DamageTypeClass->IsChildOf<UFireDamageType>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire damage detected!"));
			// Add your fire visual effect here
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Other damage type: %s"), *DamageEvent.DamageTypeClass->GetName());
			// Add other damage type handling here
		}
	}

	return ActualDamage;
}