// Fill out your copyright notice in the Description page of Project Settings.

#include "VT_ItemBase.h"

#include "Inventory/VT_InventoryComponent.h"


// Sets default values
AVT_ItemBase::AVT_ItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Location = FVT_InventoryLocation();
	Size = FIntVector2();
}

// Called when the game starts or when spawned
void AVT_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVT_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVT_ItemBase::OnAddedIntoCargo_Implementation(UVT_InventoryComponent* inv, int slotX, int slotY)
{
}

void AVT_ItemBase::OnRemovedFromCargo_Implementation(UVT_InventoryComponent* inv)
{
}