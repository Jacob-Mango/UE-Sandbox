// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

UCLASS()
class VEHICLETEST_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	AVehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Simulation, meta = (AllowPrivateAccess = "true"))
	TArray<class UVehicleComponent_Wheel*> Wheels;

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
	void OnItemAddedToCargo(class AItemBase* item);

	UFUNCTION()
	void OnItemRemovedFromCargo(class AItemBase* item);

	UFUNCTION()
	void OnItemAttached(class AItemBase* item);

	UFUNCTION()
	void OnItemDetached(class AItemBase* item);

private:
	void CalculateWheelSuspension(float DeltaTime, class UVehicleComponent_Wheel* wheel_comp, class AVehicle_Wheel* wheel_actor, FTransform& transform); 

	void CalculateWheelSlip(float DeltaTime, class UVehicleComponent_Wheel* wheel_comp, class AVehicle_Wheel* wheel_actor, FTransform& transform);
};
