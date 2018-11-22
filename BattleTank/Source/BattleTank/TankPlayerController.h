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
	/// Pair of float specifying the pixel coordinates that will be used to de-project
	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = .5;
	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.3333;

	ATank* GetControlledTank() const;

	float Reach = 1000000.0f;

	bool GetSightRayHitLocation(FVector&) const;
	bool GetLookVectorHitLocation(FVector &CameraWorldDirection, FVector & HitLocation) const;
	bool GetLookDirection(FVector2D &ScreenLocation, FVector &CameraWorldDirection) const;
	void AimTowardsCrosshair();
};
