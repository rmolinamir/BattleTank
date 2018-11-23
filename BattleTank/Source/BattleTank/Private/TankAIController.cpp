// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Map.h"
#include "Vector.h"
#include "Kismet/KismetSystemLibrary.h"

ATank* ATankAIController::GetAIControlledTank() const
{
	ATank* AIControlledTank = Cast<ATank>(GetPawn());
	if (!AIControlledTank) { return nullptr; }
	return AIControlledTank;
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerTank) { return nullptr; }
	return PlayerTank;
}

void ATankAIController::AimTowardsCrosshair()
{
	if (!GetPlayerTank()) { return; }
	// Get World Location through crosshair
	// If it hits the landscape
		// Tell controlled tank to aim at this point
	return;
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetAIControlledTank())
	{
		UE_LOG(LogTemp, Warning, TEXT("AITankController begin play not controller found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AITankController begin play controlling pawn: %s"), *GetAIControlledTank()->GetName())
	}

	if (!GetPlayerTank())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControlledTank at begin play not found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControlledTank at begin play controlling pawn: %s"), *GetPlayerTank()->GetName())
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// if (!GetPlayerTank()) { return; }
	TArray<AActor*> OutOverlappedActors; // OverlappedActors array returned from SphereOverlapActors
	if (GenerateSphereOverlapActors(OutOverlappedActors))
	{
		TPair<const AActor *, float> Actor; // Actors TPair that will serve similarly to a hashtable to filter the nearest actor
		const AActor* ClosestActor = nullptr; // Nearest actor
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
				ClosestActor = OverlappedActor; // Saves new actor in a variable for later use
			}
			else if (Distance < Actor.Value) // If the distance is lesser than the recorded lowest distance
			{
				/// Actors.Empty(); // Empty TMap
				/// Actors.Add(OverlappedActor, Distance); // Add new lowest distance
				Actor.Key = OverlappedActor; // Add new lowest distance
				Actor.Value = Distance; // Add new lowest distance
				ClosestActor = OverlappedActor; // Saves new actor in a variable for later use
			}
		}
		if (Distance > 0 && ClosestActor != nullptr) // Protection against nullptr
		{
			/// UE_LOG(LogTemp, Warning, TEXT("%s's closest actor is %s nd the distance is equal to: %f"), *GetAIControlledTank()->GetName(), *ClosestActor->GetName(), Actors[ClosestActor])
			GetAIControlledTank()->AimAt(ClosestActor->GetActorLocation());
		}
		return;
	}	
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
		10000.0f, // Radius
		ObjectTypes,
		nullptr,
		IgnorePawn, // Ignore this actor
		OutOverlappedActors
	);
	return bSphere;
}