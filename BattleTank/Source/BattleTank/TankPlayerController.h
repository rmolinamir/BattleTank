// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tank.h"
#include "TankPlayerController.generated.h" /// Must be the last include

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	ATank* GetControlledTank() const;


	// Start the tank moving the barrel so that a shot would hit
	// where the crosshair intersects the world

	float Reach = 1000000.0f;

	UPROPERTY(EditAnywhere)
	FVector PlayerViewPointLocation;

	UPROPERTY(EditAnywhere)
	FRotator PlayerViewPointRotator;

	const void GetPlayerControllerVectors(FVector &PlayerViewPointLocation, FRotator &PlayerViewPointRotation);

	bool GetSightRayHitLocation(FVector&, FVector, FRotator) const;
	void AimTowardsCrosshair();
};
