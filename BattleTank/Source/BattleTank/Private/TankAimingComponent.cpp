// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealMathUtility.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, 
	// and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	if (!ensure(BarrelToSet && TurretToSet)) { return;  }
	Barrel = BarrelToSet;
	Turret = TurretToSet;

}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret)) { return; }
	/*static bool SuggestProjectileVelocity
	(
		const UObject * WorldContextObject,
		FVector & TossVelocity,
		FVector StartLocation,
		FVector EndLocation,
		float TossSpeed,
		bool bHighArc,
		float CollisionRadius,
		float OverrideGravityZ,
		ESuggestProjVelocityTraceOption::Type TraceOption, /// ::DoNotTrace to avoid BugTracing
		const FCollisionResponseParams & ResponseParam, /// OPTIONAL
		const TArray < AActor * > & ActorsToIgnore, /// OPTIONAL
		bool bDrawDebug // OPTIONAL
	)*/
	FVector OutLaunchVelocity;
	TArray < AActor * > ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		Barrel->GetSocketLocation(FName("Projectile")),
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace // Parameter must be present to prevent bug
	);
	if (bHaveAimSolution)
	{
		FVector ProjectileDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrel(ProjectileDirection);
	}
	return;

}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTankAimingComponent::MoveBarrel(FVector ProjectileDirection)
{
	if (!ensure(Barrel && Turret)) { return; }
	// Work-out difference between current barrel rotation, and AimDirection
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = ProjectileDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else // Avoid going the long-way around
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
	return;

}

void UTankAimingComponent::Fire() const
{
	if (!ensure(Barrel)) { return;  }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (isReloaded)
	{
		//// Spawn projectile at the socket location on the barrel
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")), // FVector Location
			Barrel->GetSocketRotation(FName("Projectile")) // FRotator Rotation
			);
		if (!ensure(Projectile)) { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
		
}
