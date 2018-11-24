// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

#include "UnrealMathUtility.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);
	// Move the barrel the right amount this frame
	// Given a max elevation speed, and the frame time
	float YawRotationChange = RelativeSpeed * MaxYawRotationDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float RawNewYawRotation = RelativeRotation.Yaw + YawRotationChange;
	SetRelativeRotation(FRotator(0, RawNewYawRotation, 0));
}
