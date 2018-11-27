// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" /// Must be the last include

// Forward declarations
class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank* GetControlledTank() const;

private:
	/// Pair of float specifying the pixel coordinates that will be used to de-project
	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = .5;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.3333;

	/// Aiming Logic local variables
	float Reach = 12500.0f;
	bool GetSightRayHitLocation(FVector&) const;
	bool GetLookVectorHitLocation(FVector &CameraWorldDirection, FVector & HitLocation) const;
	bool GetLookDirection(FVector2D &ScreenLocation, FVector &CameraWorldDirection) const;
	void AimTowardsCrosshair();
};
