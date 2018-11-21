// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());

}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FString Tank = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("TankPlayerController begin play controlling pawn: %s"), *Tank)
}
