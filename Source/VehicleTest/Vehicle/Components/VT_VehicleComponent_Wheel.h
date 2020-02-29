// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "VT_VehicleComponent_Wheel.generated.h"


UCLASS( ClassGroup=("Vehicle Components"), meta=(BlueprintSpawnableComponent) )
class VEHICLETEST_API UVT_VehicleComponent_Wheel : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVT_VehicleComponent_Wheel();

	// Suspension

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
	float SuspensionLength = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
	float SuspensionSpring = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
	float SuspensionDamp = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension")
	float SuspensionCompression;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension")
	float SuspensionCompressionPrevious;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector SuspensionForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector SuspensionApplyForce;

	// Steering

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float MaxWheelAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float WheelAngle = 0.0f;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
