// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

ATank* ATankAIController::GetAIControlledTank() const
{
	ATank* AIControlledTank = Cast<ATank>(GetPawn());
	if (!AIControlledTank) { return nullptr; }
	return AIControlledTank;
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerTank) { return nullptr; }
	return PlayerTank;
}

void ATankAIController::AimTowardsCrosshair()
{
	if (!GetPlayerTank()) { return; }
	// Get World Location through crosshair
	// If it hits the landscape
		// Tell controlled tank to aim at this point
	return;
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetAIControlledTank())
	{
		UE_LOG(LogTemp, Warning, TEXT("AITankController begin play not controller found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AITankController begin play controlling pawn: %s"), *GetAIControlledTank()->GetName())
	}

	if (!GetPlayerTank())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControlledTank at begin play not found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControlledTank at begin play controlling pawn: %s"), *GetPlayerTank()->GetName())
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
