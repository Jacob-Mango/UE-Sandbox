// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "VT_Vehicle.generated.h"

UCLASS()
class VEHICLETEST_API AVT_Vehicle : public APawn
{
	GENERATED_BODY()

public:
	AVT_Vehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UVT_InventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Simulation, meta = (AllowPrivateAccess = "true"))
	TArray<class UVT_VehicleComponent_Wheel*> Wheels;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;
protected:
	void InputAccelerator(float Value);

	void InputSteering(float Value);

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	UFUNCTION()
	void OnItemAddedToCargo(class AVT_ItemBase* item);

	UFUNCTION()
	void OnItemRemovedFromCargo(class AVT_ItemBase* item);

	UFUNCTION()
	void OnItemAttached(class AVT_ItemBase* item);

	UFUNCTION()
	void OnItemDetached(class AVT_ItemBase* item);

private:
	void CalculateWheelSuspension(float DeltaTime, class UVT_VehicleComponent_Wheel* wheel_comp, class AVT_Vehicle_Wheel* wheel_actor, FTransform& transform);

	void CalculateWheelSlip(float DeltaTime, class UVT_VehicleComponent_Wheel* wheel_comp, class AVT_Vehicle_Wheel* wheel_actor, FTransform& transform);
};
