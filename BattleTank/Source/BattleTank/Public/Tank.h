// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // Put includes above

// Forward declarations
class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	void SetBarrelReference();

	/*UFUNCTION(BlueprintCallable, Category = "Actions")
	void Fire() const;*/

	void AimAt(FVector HitLocation);

private:
	// Sets default values for this pawn's properties
	ATank();

	UTankAimingComponent* TankAimingComponent = nullptr;

	virtual void BeginPlay() override;

	/*UFUNCTION(BluepreintCallable, Category = "Firing")
	void Fire() const;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 10000;

	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	mutable double LastFireTime = 0;*/

};
