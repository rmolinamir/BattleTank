// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Map.h"
#include "Vector.h"
#include "Kismet/KismetSystemLibrary.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!ensure(GetAIControlledTank()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AITankController begin play not controller found"))
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair(); // Constantly aim towards crosshair, which detects all actors in the map
	if (!Target.Key) // If for some reason the sphere fails, move to the player
	{
		MoveToActor(GetPlayerTank(), AcceptanceRadius); // TODO check radius is in cm
		GetAIControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
	else if (Target.Value < FiringRange)
	{
		/// GetAIControlledTank()->Fire();
	}
	else
	{
		MoveToActor(Target.Key, AcceptanceRadius); // TODO check radius is in cm
	}
	return;

}

// Generates a sphere around the AI controlled tank that detects all actors in it, returns array with overlapping actors and boolean status of the sphere
bool ATankAIController::GenerateSphereOverlapActors(TArray<AActor*> &OutOverlappedActors) const
{
	bool bSphere; // boolean type value that indicates if there is any overlap
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ObjectTypes to restrict results to only static or only dynamic
	TArray<AActor*> IgnorePawn; // Ignore this actor
	IgnorePawn.Add(GetPawn());

	/// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)); /// WorldDynamic is for Actor types that will be moving under the influence of animation or code; kinematic. Lifts and doors are good examples of WorldDynamic Actors.
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)); /// Any Actor that will be moving due to the physics simulation.
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); /// Any player controlled entity should have the Pawn type. The player's character is a good example of an Actor that should receive the Pawn Object Type.

	bSphere = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), // World context
		GetAIControlledTank()->GetActorLocation(), // Center of Sphere
		SphereRadius, // Radius
		ObjectTypes,
		nullptr,
		IgnorePawn, // Ignore this actor
		OutOverlappedActors
	);
	return bSphere;
}

ATank* ATankAIController::GetAIControlledTank() const
{
	ATank* AIControlledTank = Cast<ATank>(GetPawn());
	if (!ensure(AIControlledTank)) { return nullptr; }
	return AIControlledTank;
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!ensure(PlayerTank)) { return nullptr; }
	return PlayerTank;
}

void ATankAIController::AimTowardsCrosshair()
{
	/// if (!GetPlayerTank()) { return; }
	TArray<AActor*> OutOverlappedActors; // OverlappedActors array returned from SphereOverlapActors
	if (GenerateSphereOverlapActors(OutOverlappedActors))
	{
		TPair<AActor *, float> Actor; // Actors TPair that will serve similarly to a hashtable to filter the nearest actor
		float Distance; // Distance (not the nearest distance)

		for (const auto &OverlappedActor : OutOverlappedActors) // For every Actor inside the sphere
		{
			// Calculates the Distance between two vectors by substracting them then calculating the length of the resulting vector (its modulus)
			/// Distance = (OverlappedActor->GetActorLocation() - GetAIControlledTank()->GetActorLocation()).Size();
			Distance = FVector::Distance(OverlappedActor->GetActorLocation(), GetAIControlledTank()->GetActorLocation());
			if (!Actor.Key) // If if's the first loop
			{
				Actor.Key = OverlappedActor; // Add new lowest distance
				Actor.Value = Distance; // Add new lowest distance
			}
			else if (Distance < Actor.Value) // If the distance is lesser than the recorded lowest distance
			{
				Actor.Key = OverlappedActor; // Add new lowest distance
				Actor.Value = Distance; // Add new lowest distance
			}
		}
		Target = Actor;
		GetAIControlledTank()->AimAt(Target.Key->GetActorLocation());
	}
	else
	{
		Target.Key = nullptr;
		Target.Value = 0.0f;
	}
	return;
}