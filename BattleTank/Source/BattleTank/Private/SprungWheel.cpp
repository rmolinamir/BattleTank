// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// No need to protect points as added at construction
	Spring = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	// Setting up root component
	SetRootComponent(Spring);

	// Set physics parameters 
	Spring->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 100);
	Spring->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 100);
	Spring->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 100);

	Spring->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	Spring->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45);
	Spring->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);

	Spring->SetLinearPositionTarget(FVector(0, 0, 0));
	Spring->SetLinearPositionDrive(false, false, true);

	Spring->SetLinearVelocityTarget(FVector(0, 0, 0));
	Spring->SetLinearVelocityDrive(false, false, true);

	Spring->SetLinearDriveParams(5000, 2000, 0);
	// End physics parameters

	// No need to protect points as added at construction
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelStaticMesh(TEXT("/Game/Tank/SprungWheel_SM.SprungWheel_SM"));
	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Spring);
	if (WheelStaticMesh.Object) {
		Wheel->SetStaticMesh(WheelStaticMesh.Object);
	}

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	// Finds attached parent actor
	bool retflag;
	SetupConstraint(retflag);
	if (retflag) return;

}

void ASprungWheel::SetupConstraint(bool &retflag)
{
	retflag = true;
	GetAttachParentActor();
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	// Finds the attached parent actor's root component
	if (!BodyRoot) { return; }
	Spring->SetConstrainedComponents(
		BodyRoot,
		NAME_None,
		Wheel,
		NAME_None
	);
	retflag = false;
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

