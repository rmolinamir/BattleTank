// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"

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
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);

}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Calculate slippage speed
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	// Work-out the required acceleration this fram to correct
	FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	// Calculate and apply sideways force (F = m * a)
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	if (!ensure(TankRoot)) { return; }
	FVector CorrectionForce = (CorrectionAcceleration * TankRoot->GetMass()/2); /// Divided by two because there are two tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) // TODO clamp actual throttle value so player can't over-drive
{
	FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	// Add Force at Tank Component (Which is the root component)
	Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);

}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
}