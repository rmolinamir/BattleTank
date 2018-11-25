// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

UCLASS(ClassGroup = (Rendering, Common), meta = (BlueprintSpawnableComponent)) /// , hidecategories = ("Collision"))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

};
