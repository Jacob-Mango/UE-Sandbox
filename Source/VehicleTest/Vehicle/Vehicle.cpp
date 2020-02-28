// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"

#include "Vehicle_Wheel.h"
#include "VehicleComponent_Wheel.h"

#include "../InventoryComponent.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(true);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->CargoSize = FIntVector2(10, 10);

	InventoryComponent->OnItemAddedToCargo.AddDynamic(this, &AVehicle::OnItemAddedToCargo);
	InventoryComponent->OnItemRemovedFromCargo.AddDynamic(this, &AVehicle::OnItemRemovedFromCargo);
	InventoryComponent->OnItemAttached.AddDynamic(this, &AVehicle::OnItemAttached);
	InventoryComponent->OnItemDetached.AddDynamic(this, &AVehicle::OnItemDetached);
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

}

void AVehicle::OnConstruction(const FTransform & Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnConstruction"));
	Super::OnConstruction(Transform);

	GetComponents(WheelComponents);
	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnConstruction - Success"));
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UVehicleComponent_Wheel*> processingWheels;
	
	for (int32 i = 0; i < WheelComponents.Num(); i++)
	{
		UVehicleComponent_Wheel* wheel_comp = WheelComponents[i];
		FName socket_name = wheel_comp->GetAttachSocketName();
		AVehicle_Wheel* wheel_actor = Cast<AVehicle_Wheel>(InventoryComponent->GetAttachment(socket_name));
		if (wheel_actor == NULL)
			continue;

		processingWheels.Add(wheel_comp);

		FTransform trans = MeshComponent->GetSocketTransform(socket_name);

		CalculateWheelSuspension(DeltaTime, wheel_comp, wheel_actor, trans);

		wheel_actor->MeshComponent->SetWorldTransform(trans);

		wheel_comp->SuspensionCompressionPrevious = wheel_comp->SuspensionCompression;
	}

	for (int32 i = 0; i < processingWheels.Num(); i++)
	{
		UVehicleComponent_Wheel* wheel_comp = processingWheels[i];
		
		MeshComponent->AddForceAtLocation(wheel_comp->SuspensionForce, wheel_comp->SuspensionApplyForce);
	}
}

void AVehicle::CalculateWheelSuspension(float DeltaTime, UVehicleComponent_Wheel * wheel_comp, AVehicle_Wheel * wheel_actor, FTransform & transform)
{
	const float WheelRadius = wheel_actor->WheelRadius;
	const float SuspensionLength = wheel_comp->SuspensionLength;
	const float RayLength = SuspensionLength + WheelRadius;

	const FVector StartTrace = transform.GetLocation();
	const FVector Direction = -transform.GetUnitAxis(EAxis::Z);
	const FVector EndTrace = StartTrace + Direction * RayLength;
	FHitResult result;

	FVector newWheelPosition = EndTrace;

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, -1, 0, 1);
	if (GetWorld()->LineTraceSingleByChannel(result, StartTrace, EndTrace, ECollisionChannel::ECC_WorldStatic))
	{
		float distance = FVector::Distance(StartTrace, result.ImpactPoint);

		wheel_comp->SuspensionCompression = RayLength - distance;
		if (wheel_comp->SuspensionCompression > WheelRadius)
			wheel_comp->SuspensionCompression = WheelRadius;
	}
	else
	{
		wheel_comp->SuspensionCompression = 0;
	}

	FVector springForce = result.ImpactNormal * (wheel_comp->SuspensionCompression - (SuspensionLength * 0.5)) * wheel_comp->SuspensionSpring;
	float suspCompressionVel = (wheel_comp->SuspensionCompression - wheel_comp->SuspensionCompressionPrevious) / DeltaTime;
	FVector damperForce = result.ImpactNormal * suspCompressionVel * wheel_comp->SuspensionDamp;

	newWheelPosition = StartTrace + Direction * (SuspensionLength - wheel_comp->SuspensionCompression);

	wheel_comp->SuspensionForce = (springForce + damperForce) * MeshComponent->GetMass();
	wheel_comp->SuspensionApplyForce = newWheelPosition;

	transform.SetLocation(newWheelPosition);
}

void AVehicle::OnItemAddedToCargo(AItemBase* item)
{
}

void AVehicle::OnItemRemovedFromCargo(AItemBase* item)
{
}

void AVehicle::OnItemAttached(AItemBase* item)
{
	//UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemAttached"));
	//
	//AVehicle_Wheel* wheel = Cast<AVehicle_Wheel>(item);
	//if (wheel == NULL)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemAttached - Fail 1"));
	//	return;
	//}
	//
	//int slot = item->Location.AttachmentSlot;
	//
	//UE_LOG(LogTemp, Warning, TEXT( "Num: %d Slot: %d" ), WheelComponents.Num(), slot);
	//
	//if (slot >= 0 && slot < WheelComponents.Num())
	//{
	//	WheelComponents[slot]->Wheel = wheel;
	//	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemAttached - Success"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemAttached - Fail 2"));
	//}
}

void AVehicle::OnItemDetached(AItemBase* item)
{
	//UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemDetached"));
	//
	//int slot = item->Location.AttachmentSlot;
	//
	//if (slot >= 0 && slot < WheelComponents.Num())
	//{
	//	WheelComponents[slot]->Wheel = NULL;
	//	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemDetached - Success"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AVehicle::OnItemDetached - Fail 1"));
	//}
}