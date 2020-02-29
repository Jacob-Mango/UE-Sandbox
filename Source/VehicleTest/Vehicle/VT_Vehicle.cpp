// Fill out your copyright notice in the Description page of Project Settings.

#include "VT_Vehicle.h"

#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"

#include "Vehicle/VT_Vehicle_Wheel.h"
#include "Vehicle/Components/VT_VehicleComponent_Wheel.h"
#include "Inventory/VT_InventoryComponent.h"

// Sets default values
AVT_Vehicle::AVT_Vehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(true);

	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(Mesh);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	Inventory = CreateDefaultSubobject<UVT_InventoryComponent>(TEXT("Inventory"));
	Inventory->CargoSize = FIntVector2(10, 10);

	Inventory->OnItemAddedToCargo.AddDynamic(this, &AVT_Vehicle::OnItemAddedToCargo);
	Inventory->OnItemRemovedFromCargo.AddDynamic(this, &AVT_Vehicle::OnItemRemovedFromCargo);
	Inventory->OnItemAttached.AddDynamic(this, &AVT_Vehicle::OnItemAttached);
	Inventory->OnItemDetached.AddDynamic(this, &AVT_Vehicle::OnItemDetached);
}

void AVT_Vehicle::InputAccelerator(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("InputAccelerator %f"), Value);
}

void AVT_Vehicle::InputSteering(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("InputSteering %f"), Value);
}

// Called when the game starts or when spawned
void AVT_Vehicle::BeginPlay()
{
	Super::BeginPlay();

}

void AVT_Vehicle::SetupPlayerInputComponent(UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

	inputComponent->BindAxis("VehicleAccelerator", this, &AVT_Vehicle::InputAccelerator);
	inputComponent->BindAxis("VehicleSteering", this, &AVT_Vehicle::InputSteering);
}

void AVT_Vehicle::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	GetComponents(Wheels);
}

// Called every frame
void AVT_Vehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UVT_VehicleComponent_Wheel*> processingWheels;
	
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		UVT_VehicleComponent_Wheel* wheel_comp = Wheels[i];
		FName socket_name = wheel_comp->GetAttachSocketName();
		AVT_Vehicle_Wheel* wheel_actor = Cast<AVT_Vehicle_Wheel>(Inventory->GetAttachment(socket_name));
		if (wheel_actor == NULL)
			continue;

		processingWheels.Add(wheel_comp);

		FTransform trans = MeshComponent->GetSocketTransform(socket_name);

		trans.SetRotation(trans.TransformRotation(FQuat::MakeFromEuler(FVector(wheel_comp->WheelAngle, 0, 0))));

		CalculateWheelSuspension(DeltaTime, wheel_comp, wheel_actor, trans);

		wheel_actor->MeshComponent->SetWorldTransform(trans);

		wheel_comp->SuspensionCompressionPrevious = wheel_comp->SuspensionCompression;
	}

	for (int32 i = 0; i < processingWheels.Num(); i++)
	{
		UVT_VehicleComponent_Wheel* wheel_comp = processingWheels[i];
		
		MeshComponent->AddForceAtLocation(wheel_comp->SuspensionForce, wheel_comp->SuspensionApplyForce);
	}
}

void AVT_Vehicle::CalculateWheelSuspension(float DeltaTime, UVT_VehicleComponent_Wheel* wheel_comp, AVT_Vehicle_Wheel* wheel_actor, FTransform & transform)
{
	const float WheelRadius = wheel_actor->WheelRadius;
	const float SuspensionLength = wheel_comp->SuspensionLength;
	const float RayLength = SuspensionLength + WheelRadius;

	const FVector StartTrace = transform.GetLocation();
	const FVector Direction = -transform.GetUnitAxis(EAxis::Z);
	const FVector EndTrace = StartTrace + Direction * RayLength;
	FHitResult result;

	FVector newWheelPosition = EndTrace;
	FVector suspensionApplyDir = -Direction;

	DrawDebugLine(GetWorld(), StartTrace, StartTrace + transform.GetUnitAxis(EAxis::X) * RayLength, FColor::Red, false, -1, 0, 1);
	DrawDebugLine(GetWorld(), StartTrace, StartTrace + transform.GetUnitAxis(EAxis::Y) * RayLength, FColor::Blue, false, -1, 0, 1);

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, -1, 0, 1);
	if (GetWorld()->LineTraceSingleByChannel(result, StartTrace, EndTrace, ECollisionChannel::ECC_WorldStatic))
	{
		float distance = FVector::Distance(StartTrace, result.ImpactPoint);

		wheel_comp->SuspensionCompression = RayLength - distance;
		if (wheel_comp->SuspensionCompression > WheelRadius)
			wheel_comp->SuspensionCompression = WheelRadius;

		suspensionApplyDir = result.ImpactNormal;
	}
	else
	{
		wheel_comp->SuspensionCompression = 0;
	}

	FVector springForce = suspensionApplyDir * (wheel_comp->SuspensionCompression - (SuspensionLength * 0.5)) * wheel_comp->SuspensionSpring;
	float suspCompressionVel = (wheel_comp->SuspensionCompression - wheel_comp->SuspensionCompressionPrevious) / DeltaTime;
	FVector damperForce = suspensionApplyDir * suspCompressionVel * wheel_comp->SuspensionDamp;

	newWheelPosition = StartTrace + Direction * (SuspensionLength - wheel_comp->SuspensionCompression);

	wheel_comp->SuspensionForce = (springForce + damperForce) * MeshComponent->GetMass();
	wheel_comp->SuspensionApplyForce = newWheelPosition;

	transform.SetLocation(newWheelPosition);
}

void AVT_Vehicle::CalculateWheelSlip(float DeltaTime, UVT_VehicleComponent_Wheel* wheel_comp, AVT_Vehicle_Wheel* wheel_actor, FTransform & transform)
{

}

void AVT_Vehicle::OnItemAddedToCargo(AVT_ItemBase* item)
{
}

void AVT_Vehicle::OnItemRemovedFromCargo(AVT_ItemBase* item)
{
}

void AVT_Vehicle::OnItemAttached(AVT_ItemBase* item)
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

void AVT_Vehicle::OnItemDetached(AVT_ItemBase* item)
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