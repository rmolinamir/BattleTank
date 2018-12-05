// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // Put includes above

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	void SetBarrelReference();
	
	// Return current health as a percentage of starting health, between 0 and 1
	UFUNCTION(BlueprintPure, Category = "Setup")
	float GetHealthPercent() const;

	// Delecate
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FTankDelegate OnDeathDelegate;

protected:
	// Called by the engine when tank takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 StartingHealth = 100.0f; // Default value of 100.0f

private:
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetCurrentHealth(int32 Health);
		
	UPROPERTY(VisibleAnywhere, Category = "Gameplay Statistics")
	int32 CurrentHealth;

};
