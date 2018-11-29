// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "TankAimingComponent.h"	

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!ensure(GetPawn())) { return; }
	TankAimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(TankAimingComponent))
	{
		// FoundAimintComponent() BlueprintImplementableEvent 
		// to create BP ref and create aiming reticule widget UI
		FoundAimingComponent(TankAimingComponent); 
	}
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

// Get World Location through crosshair
void ATankPlayerController::AimTowardsCrosshair()
{
	if (!ensure(GetControlledTank())) { return; }

	FVector OutHitLocation; // Out parameter
	// If it hits the landscape
	if (GetSightRayHitLocation(OutHitLocation)) /// Has "side-effect", it line traces
	{
		// Tell controlled tank to aim at this point
		TankAimingComponent->AimAt(OutHitLocation);
	}
	return;
}

// Get world location through crosshair, true if it hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position in pixel coordinates
	/*int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);
	FVector CameraWorldDirection;
	if (GetLookDirection(ScreenLocation, CameraWorldDirection)) /// if it exists and see what we hit (up to max range)
	{
		return GetLookVectorHitLocation(CameraWorldDirection, OutHitLocation);
	}
	OutHitLocation = FVector(0);
	return false;*/

	// Alternative Method for Line-Tracing with GetHitResultAtScreenPosition()
	bool bHit;
	FHitResult Hit2D;
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);
	bHit = GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(
		ScreenLocation,
		ECollisionChannel::ECC_Visibility,
		true,
		Hit2D
	);
	if (bHit && Cast<ATank>(Hit2D.Actor) != GetControlledTank())
	{
		OutHitLocation = Hit2D.ImpactPoint;
	}
	else
	{
		OutHitLocation = FVector(0);
	}
	return bHit;
}

// Line-trace along CameraWorldDirection
bool ATankPlayerController::GetLookVectorHitLocation(FVector &CameraWorldDirection, FVector & OutHitLocation) const
{
	/// Get player view point this tick
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); /// Setup query parameters
	/// Ray-cast out to reach distance
	FHitResult Hit;
	/*GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerCameraManager->GetCameraLocation(), /// Returns the vectors of the first player controller, by modifying their memory address
		PlayerCameraManager->GetCameraLocation() + CameraWorldDirection * Reach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);*/
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		PlayerCameraManager->GetCameraLocation(), /// Returns the vectors of the first player controller, by modifying their memory address
		PlayerCameraManager->GetCameraLocation() + CameraWorldDirection * Reach,
		ECollisionChannel::ECC_Visibility
	);
	OutHitLocation = Hit.Location;
	if (Hit.IsValidBlockingHit() && Cast<ATank>(Hit.Actor) != GetControlledTank())
	{
		return true;
	}
	OutHitLocation = FVector(0);
	return false;
}

// "De-project" the screen position of the crosshair to a world direction
bool ATankPlayerController::GetLookDirection(FVector2D &ScreenLocation, FVector &CameraWorldDirection) const
{
	FVector CameraWorldLocation; /// To be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		CameraWorldDirection
	);
}