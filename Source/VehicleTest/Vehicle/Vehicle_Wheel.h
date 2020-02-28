// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../ItemBase.h"

#include "Vehicle_Wheel.generated.h"

UCLASS()
class VEHICLETEST_API AVehicle_Wheel : public AItemBase
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AVehicle_Wheel();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float WheelRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slip")
	float Grip = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slip")
	float MaxGrip = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slip")
	float MaxLatGrip = 2.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
