// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

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

	Spring->SetLinearDriveParams(500, 200, 0);
	// End physics parameters

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(Spring);

	// No need to protect points as added at construction
	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	// Setting up root component
	AxleWheelConstraint->SetupAttachment(Axle);

	/// No need to protect points as added at construction
	/// static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelStaticMesh(TEXT("/Game/Tank/SprungWheel_SM.SprungWheel_SM"));
	Wheel1 = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel1->SetupAttachment(Axle);
	/*if (WheelStaticMesh.Object) {
		Wheel->SetStaticMesh(WheelStaticMesh.Object);
	}*/
	

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	// Finds attached parent actor
	bool retflag;
	SetupConstraint(retflag);
	if (retflag) return;
	// Activate collision
	Wheel1->SetNotifyRigidBodyCollision(true);
	Wheel1->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Canceling the force
		TotalForceMagnitude = 0;
	}

}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitude += ForceMagnitude;
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
		Axle,
		NAME_None
	);
	if (!AxleWheelConstraint) { return; }
	AxleWheelConstraint->SetConstrainedComponents(
		Axle,
		NAME_None,
		Wheel1,
		NAME_None
	);
	retflag = false;
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (!(Hit.GetActor())) { return;  }
	Wheel1->AddForce(Axle->GetForwardVector() * TotalForceMagnitude);

}

//void ASprungWheel::SetNumberOfWheels(int32 NumberOfWheels)
//{
//	this->NumberOfWheels = NumberOfWheels;
//
//}