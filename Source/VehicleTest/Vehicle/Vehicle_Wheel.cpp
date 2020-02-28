// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle_Wheel.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AVehicle_Wheel::AVehicle_Wheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(false);

	Location = FInventoryLocation();
	Size = FIntVector2();
}

// Called when the game starts or when spawned
void AVehicle_Wheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehicle_Wheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

