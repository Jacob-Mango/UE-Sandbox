// Fill out your copyright notice in the Description page of Project Settings.

#include "VT_Vehicle_Wheel.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AVT_Vehicle_Wheel::AVT_Vehicle_Wheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(false);

	Location = FVT_InventoryLocation();
	Size = FIntVector2();
}

// Called when the game starts or when spawned
void AVT_Vehicle_Wheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVT_Vehicle_Wheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

