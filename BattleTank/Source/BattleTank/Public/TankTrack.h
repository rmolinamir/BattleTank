// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

// Forwarded declarations
class ASprungWheel;

UCLASS(ClassGroup = (Rendering, Common), meta = (BlueprintSpawnableComponent)) /// , hidecategories = ("Collision"))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	// Max force per track, in Newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 40000000.0; // Assume 40ton tank, and 1g acceleration

protected:
	/// Called when the game starts or when spawned
	/// virtual void OnCreatePhysicsState() override;

private:
	// Sets default values for this component's properties
	UTankTrack();

	/// void ApplySidewayForce();

	void DriveTrack(float CurrentThrottle);

	/// UPhysicalMaterial* PhysicalMaterialObj = nullptr;

	/// UFUNCTION(BlueprintCallable, Category = "Setup")
	/// void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	TArray<ASprungWheel*> GetWheels() const;

};
