// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InventoryLocation.h"

#include "ItemBase.generated.h"

UCLASS()
class VEHICLETEST_API AItemBase : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FIntVector2 Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FInventoryLocation Location;
		
	UFUNCTION(BlueprintNativeEvent)
	void OnAddedIntoCargo(class UInventoryComponent* inv, int slotX, int slotY);

	UFUNCTION(BlueprintNativeEvent)
	void OnRemovedFromCargo(class UInventoryComponent* inv);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
