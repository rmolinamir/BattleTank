// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.h" // To Add Driving Force
#include "SpawnPoint.h" // For GetWheels()

/**
* TankTrack is used to set maximum driving force, and to apply forces to the tank.
*/

// Sets default values for this component's properties
UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
	/// OnComponentHit.AddDynamic(this, &UTankTrack::OnHit); /// First step to add OnHit events
	/// this->SetNotifyRigidBodyCollision(true); // To set Simulate Hit Events true by default on the Blueprint
	// Finds the static mesh to be set
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Tank/tank_fbx_Track.tank_fbx_Track'"));
	// If there is a find, it sets the static mesh
	if (StaticMesh.Object) {
		this->SetStaticMesh(StaticMesh.Object);
	}
	/// Finds the physical material to be set OnCreatePhysicsState()
	/*static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysicalMaterial(TEXT("/Game/Tank/Track.Track"));
	PhysicalMaterialObj = PhysicalMaterial.Object;*/

}

/*void UTankTrack::OnCreatePhysicsState()
{
	Super::OnCreatePhysicsState();
	// If there is a find, it sets the physical material
	if (PhysicalMaterialObj) {
			UE_LOG(LogTemp, Warning, TEXT("FoundMaterial"))
			this->SetPhysMaterialOverride(PhysicalMaterialObj);
	}
	return;
	
}*/

/*void UTankTrack::ApplySidewayForce()
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

}*/

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, +1);
	DriveTrack(CurrentThrottle);

}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	float ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	float ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels)
	{
		if (!ensure(Wheel)) { continue; }
		Wheel->AddDrivingForce(ForcePerWheel);
		
	}
	return;

}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> SprungWheels;
	TArray <USceneComponent*> SpawnPoints;
	this->GetChildrenComponents(true, SpawnPoints);
	for (USceneComponent* SpawnPoint : SpawnPoints)
	{

		/*ASprungWheel* SprungWheel = Cast<ASprungWheel>(Cast<USpawnPoint>(SpawnPoint)->GetSpawnPoint());
		if (!SprungWheel) { continue; }
		UE_LOG(LogTemp, Warning, TEXT("SPAWNPOINT FOUND"))
		SprungWheels.Add(SprungWheel);*/
		auto SpawnPointChild = Cast<USpawnPoint>(SpawnPoint);
		if (!SpawnPointChild) { continue;  }

		AActor* SpawnedChild = SpawnPointChild->GetSpawnPoint();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) { continue; }

		SprungWheels.Add(SprungWheel);
	}
	return SprungWheels;

}

//void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
//{
//	DriveTrack();
//	ApplySidewayForce();
//	CurrentThrottle = 0;
//
//}