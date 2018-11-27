// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}

void ATank::SetTankAimingComponent(UTankAimingComponent* TankAimingComponentToSet)
{
	TankAimingComponent = TankAimingComponentToSet;
}

//void ATank::Fire() const
//{
//	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
//	if (Barrel && isReloaded)
//	{
//		//// Spawn projectile at the socket location on the barrel
//		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
//			ProjectileBlueprint,
//			Barrel->GetSocketLocation(FName("Projectile")), // FVector Location
//			Barrel->GetSocketRotation(FName("Projectile")) // FRotator Rotation
//			);
//		Projectile->LaunchProjectile(LaunchSpeed);
//		LastFireTime = FPlatformTime::Seconds();
//	}
//}

void ATank::AimAt(FVector HitLocation)
{
	if (!TankAimingComponent) { return; }
	TankAimingComponent->AimAt(HitLocation);

}
