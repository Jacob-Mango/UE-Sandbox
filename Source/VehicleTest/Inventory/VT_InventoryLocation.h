// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "VT_InventoryLocation.generated.h"

USTRUCT(BlueprintType)
struct FIntVector2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y;

	FIntVector2()
	{
		X = 0;
		Y = 0;
	}

	FIntVector2(int x, int y)
	{
		X = x;
		Y = y;
	}
};


USTRUCT(BlueprintType)
struct FVT_InventoryLocation
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UVT_InventoryComponent* Owning;

	// if owning is not null, AttachmentSlot is checked first for if 
	// it equals zero or greater, otherwise CargoPosition is used.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName AttachmentSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FIntVector2 CargoPosition;

	static FVT_InventoryLocation Cargo(class UVT_InventoryComponent* owner, FIntVector2 pos)
	{
		FVT_InventoryLocation location;

		location.Owning = owner;
		location.AttachmentSlot = FName();
		location.CargoPosition = pos;

		return location;
	}

	static FVT_InventoryLocation Attachment(class UVT_InventoryComponent* owner, FName slot)
	{
		FVT_InventoryLocation location;

		location.Owning = owner;
		location.AttachmentSlot = slot;
		location.CargoPosition = FIntVector2(-1, -1);

		return location;
	}
};
