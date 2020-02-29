// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Inventory/VT_InventoryLocation.h"

#include "VT_ItemBase.generated.h"

UCLASS()
class VEHICLETEST_API AVT_ItemBase : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AVT_ItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FIntVector2 Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FVT_InventoryLocation Location;
		
	UFUNCTION(BlueprintNativeEvent)
	void OnAddedIntoCargo(class UVT_InventoryComponent* inv, int slotX, int slotY);

	UFUNCTION(BlueprintNativeEvent)
	void OnRemovedFromCargo(class UVT_InventoryComponent* inv);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
