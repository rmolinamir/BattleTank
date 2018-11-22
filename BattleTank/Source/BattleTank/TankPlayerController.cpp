// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Camera/PlayerCameraManager.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController begin play not controller found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController begin play controlling pawn: %s"), *ControlledTank->GetName())
	}
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
	/// Draw a red 
	
	GetPlayerControllerVectors(PlayerViewPointLocation, PlayerViewPointRotator);

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	FVector OutHitLocation;
	if (GetSightRayHitLocation(OutHitLocation, PlayerViewPointLocation, PlayerViewPointRotator);)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT CONFIRMED. HitLocation is: %s"), *OutHitLocation.ToString())
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

/// Returns the vectors of the first player controller, by modifying their memory address
const void ATankPlayerController::GetPlayerControllerVectors(FVector &PlayerViewPointLocation, FRotator &PlayerViewPointRotation)
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation, FVector PlayerViewPointLocation, FRotator PlayerViewPointRotator) const
{
	/// Get player view point this tick
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	HitLocation = Hit.Location;
	if (Hit.IsValidBlockingHit() && Cast<ATank>(Hit.Actor) != GetControlledTank())
	{
		return true;
	}
	return false;
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }
	// Get World Location through crosshair
	// If it hits the landscape
		// Tell controlled tank to aim at this point
	return;
}