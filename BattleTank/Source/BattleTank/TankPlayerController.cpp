// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"
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
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

// Get World Location through crosshair
void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector OutHitLocation; // Out parameter
	// If it hits the landscape
	if (GetSightRayHitLocation(OutHitLocation)) /// Has "side-effect", it line traces
	{
		// Tell controlled tank to aim at this point
		/// UE_LOG(LogTemp, Warning, TEXT("HIT CONFIRMED. HitLocation is: %s"), *OutHitLocation.ToString())
		GetControlledTank()->AimAt(OutHitLocation);
	}
	return;
}

// Get world location through crosshair, true if it hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);
	FVector CameraWorldDirection;
	if (GetLookDirection(ScreenLocation, CameraWorldDirection)) /// if it exists and see what we hit (up to max range)
	{
		return GetLookVectorHitLocation(CameraWorldDirection, OutHitLocation);
	}
	OutHitLocation = FVector(0);
	return false;

	// Alternative Method for Line-Tracing with GetHitResultAtScreenPosition()
	/*bool bHit;
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
		UE_LOG(LogTemp, Warning, TEXT("Location: %s | Targeting: %s"), *Hit2D.ImpactPoint.ToString(), *Hit2D.Actor->GetName())
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Location: NULL | Targeting: NOTHING"))
	}*/
}

// Line-trace along CameraWorldDirection
bool ATankPlayerController::GetLookVectorHitLocation(FVector &CameraWorldDirection, FVector & OutHitLocation) const
{
	/// Get player view point this tick
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); /// Setup query parameters
	/// Ray-cast out to reach distance
	FHitResult Hit;
	FHitResult Hit2D;
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