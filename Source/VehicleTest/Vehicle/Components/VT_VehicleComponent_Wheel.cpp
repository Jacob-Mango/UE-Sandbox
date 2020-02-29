// Fill out your copyright notice in the Description page of Project Settings.


#include "VT_VehicleComponent_Wheel.h"

// Sets default values for this component's properties
UVT_VehicleComponent_Wheel::UVT_VehicleComponent_Wheel()
{
	// Super();

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVT_VehicleComponent_Wheel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVT_VehicleComponent_Wheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

