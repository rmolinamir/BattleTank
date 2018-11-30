// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

/**
* TankTrack is used to set maximum driving force, and to apply forces to the tank.
*/

// Sets default values for this component's properties
UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, 
	// and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

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
