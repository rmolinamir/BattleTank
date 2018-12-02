// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();

}

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, 
	// and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Sets the ProjectileBlueprint equal to the Projectile_BP 
	// blueprint file on editor due to bug in the engine
	static ConstructorHelpers::FClassFinder<AProjectile> Projectile(TEXT("/Game/Projectile/Projectile_BP"));
	if (Projectile.Class)
	{
		ProjectileBlueprint = Projectile.Class;
	}
	
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Ammo <= 0)
	{
		FiringStatus = EFiringStatus::OutOfAmmo;
	} else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}

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
	FVector OutLaunchVelocity;
	TArray < AActor * > ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this, /// const UObject * WorldContextObject,
		OutLaunchVelocity, /// FVector & TossVelocity
		Barrel->GetSocketLocation(FName("Projectile")), /// FVector StartLocation
		HitLocation, /// FVector EndLocation
		LaunchSpeed, /// float TossSpeed
		false, /// bool bHighArc
		0.f, /// float CollisionRadius
		0.f, /// float OverrideGravityZ
		ESuggestProjVelocityTraceOption::DoNotTrace // Parameter must be present to prevent bug 
	);
	if (bHaveAimSolution)
	{
		ProjectileDirection = OutLaunchVelocity.GetSafeNormal(); /// FVector local variable
		MoveBarrel(ProjectileDirection);
	}
	return;

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

bool UTankAimingComponent::IsBarrelMoving() const
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return false; }
	return !(Barrel->GetForwardVector().Equals(ProjectileDirection, IsBarrelAimingTolerance));

}

void UTankAimingComponent::Fire() const
{

	if (FiringStatus == EFiringStatus::Locked || FiringStatus == EFiringStatus::Aiming)
	{
		if (!ensure(Barrel && ProjectileBlueprint)) { return; }
		//// Spawn projectile at the socket location on the barrel
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")), // FVector Location
			Barrel->GetSocketRotation(FName("Projectile")) // FRotator Rotation
			);
		if (!ensure(Projectile)) { return; }	
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		Ammo--;
	}
	return;

}

int32 UTankAimingComponent::GetAmmo() const
{
	return Ammo;
}

EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return FiringStatus;

}
