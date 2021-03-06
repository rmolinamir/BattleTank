// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	/// Actor* NewActor = GetWorld()->SpawnActor<AActor>(SpawnClass);
	FTransform const Transform = GetComponentTransform();
	SpawnPoint = GetWorld()->SpawnActorDeferred<AActor>(
		SpawnClass,
		Transform
		/*GetOwner(),
		GetOwner()->Instigator,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding*/
	);
	if (!SpawnPoint) { return; }
	SpawnPoint->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::FinishSpawningActor(SpawnPoint, Transform);

}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

AActor* USpawnPoint::GetSpawnPoint() const
{
	return SpawnPoint;

}

