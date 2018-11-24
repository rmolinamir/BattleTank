// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::SetBarrelReference(UStaticMeshComponent * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }

	/*static bool SuggestProjectileVelocity
	(
		const UObject * WorldContextObject,
		FVector & TossVelocity,
		FVector StartLocation,
		FVector EndLocation,
		float TossSpeed,
		bool bHighArc,
		float CollisionRadius,
		float OverrideGravityZ,
		ESuggestProjVelocityTraceOption::Type TraceOption, /// OPTIONAL
		const FCollisionResponseParams & ResponseParam, /// OPTIONAL
		const TArray < AActor * > & ActorsToIgnore, /// OPTIONAL
		bool bDrawDebug
	)*/
	FVector OutLaunchVelocity;
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		Barrel->GetSocketLocation(FName("Projectile")),
		HitLocation,
		LaunchSpeed,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHaveAimSolution)
	{
		FVector ProjectileDirection = OutLaunchVelocity.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("Firing at launch speed of: %f | Unit Direction: %s | Target location: %s"), LaunchSpeed, *ProjectileDirection.ToString(), *HitLocation.ToString())
		MoveBarrel(ProjectileDirection);
	}
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTankAimingComponent::MoveBarrel(FVector ProjectileDirection)
{
	// setup pivot point from which the barrel will rotate
	// calculate the rotation of the barrel using the FVector then save it to a local variable
	// broadcast variable to blueprint to animate rotation
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

