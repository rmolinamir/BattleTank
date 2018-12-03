// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM()
enum class EFiringStatus : uint8
{
	OutOfAmmo,
	Reloading,
	Aiming,
	Locked,
};

// Forward Declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

/**
* Holds barrel's and turret's properties and methods
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector WorldSpace);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Fire() const;
	
	EFiringStatus GetFiringStatus() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetAmmo() const;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	void MoveBarrel(FVector);

	bool IsBarrelMoving() const;
	
	FVector ProjectileDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float IsBarrelAimingTolerance = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 10000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	mutable double LastFireTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	mutable int32 Ammo = 20;
};
