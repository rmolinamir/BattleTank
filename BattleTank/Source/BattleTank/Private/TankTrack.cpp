// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"

/**
* TankTrack is used to set maximum driving force, and to apply forces to the tank.
*/

// Sets default values for this component's properties
UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit); /// First step to add OnHit events
	this->SetNotifyRigidBodyCollision(true); // To set Simulate Hit Events true by default on the Blueprint
	// Finds the static mesh to be set
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Tank/tank_fbx_Track.tank_fbx_Track'"));
	// If there is a find, it sets the static mesh
	if (StaticMesh.Object) {
		this->SetStaticMesh(StaticMesh.Object);
	}
	// Finds the physical material to be set OnCreatePhysicsState()
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysicalMaterial(TEXT("/Game/Tank/Track.Track"));
	PhysicalMaterialObj = PhysicalMaterial.Object;	

}

void UTankTrack::OnCreatePhysicsState()
{
	Super::OnCreatePhysicsState();
	// If there is a find, it sets the physical material
	if (PhysicalMaterialObj) {
			UE_LOG(LogTemp, Warning, TEXT("FoundMaterial"))
			this->SetPhysMaterialOverride(PhysicalMaterialObj);
	}
	return;
	
}

void UTankTrack::ApplySidewayForce()
{
	// Calculate slippage speed and get delta time seconds
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	// Work-out the required acceleration this fram to correct
	FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	// Calculate and apply sideways force (F = m * a)
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	if (!ensure(TankRoot)) { return; }
	FVector CorrectionForce = (CorrectionAcceleration * TankRoot->GetMass() / 2); /// Divided by two because there are two tracks
	TankRoot->AddForce(CorrectionForce);
	return;

}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, +1);
	/// DriveTrack(); /// To turn

}

void UTankTrack::DriveTrack()
{
	FVector ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	// Add Force at Tank Component (Which is the root component)
	Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);
	return;

}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewayForce();
	CurrentThrottle = 0;

}