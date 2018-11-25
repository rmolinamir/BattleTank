// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

/**
* TankTrack is used to set maximum driving force, and to apply forces to the tank.
*/
void UTankTrack::SetThrottle(float Throttle) // TODO clamp actual throttle value so player can't over-drive
{
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	// Add Force at Tank Component (Which is the root component)
	Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);
}
