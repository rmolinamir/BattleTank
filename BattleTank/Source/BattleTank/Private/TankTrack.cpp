// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

/**
* TankTrack is used to set maximum driving force, and to apply forces to the tank.
*/
void UTankTrack::SetThrottle(float Throttle)
{
	FString Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s's throttle is: %f"), *Name, Throttle)
}