// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"

void UTankMovementComponent::SetTankMovementReference(UTankMovementComponent* TankMovementToSet)
{
	TankMovement = TankMovementToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throttle) const
{
	UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward: %s's throttle is: %f"), Throttle)

}
