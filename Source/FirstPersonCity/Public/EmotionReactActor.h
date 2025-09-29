// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"
#include "Engine/DamageEvents.h"
#include "EmotionReactActor.generated.h"

class UFireDamageType;

UCLASS()
class FIRSTPERSONCITY_API AEmotionReactActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmotionReactActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Static mesh component for visual representation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** Passes control to Blueprint to implement any effects on hit. */
	UFUNCTION(BlueprintImplementableEvent, Category="Projectile", meta = (DisplayName = "On Projectile Hit"))
	void BP_OnProjectileHit(TSubclassOf<UDamageType> DamageTypeClass);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Handle incoming damage and trigger visual effects based on damage type */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


};
