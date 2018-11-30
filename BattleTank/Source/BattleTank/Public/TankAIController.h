// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

// Forward declarations
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	// How close can the AI tank get
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AcceptanceRadius = 6000.0f;

private:
	UTankAimingComponent* TankAimingComponent = nullptr;

	APawn* GetAIControlledTank() const;

	APawn* GetPlayerTank() const;

	bool GenerateSphereOverlapActors(TArray<AActor*>&) const;

	// Start the tank moving the barrel so that a shot would hit
	// where the crosshair intersects the world
	void AimTowardsCrosshair();

	// TPair containing Closest Actor and Distance from tank
	TPair<AActor *, float> Target;

	// Firing Range
	float FiringRange = 10000.0f;

	// Radius of the sphere
	float SphereRadius = 10000000.0f;

};
