// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

bool ATankPlayerController::GetSightRayHitLocation(FVector* HitLocation) const
{
	/// HitLocation = &GetControlledTank()->GetActorLocation();
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
	FVector HitLocation;
	bool HitResult = GetSightRayHitLocation(&HitLocation);
	if (HitResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("No hit results."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit result is: %s"), *HitLocation.ToString())
	}
}
