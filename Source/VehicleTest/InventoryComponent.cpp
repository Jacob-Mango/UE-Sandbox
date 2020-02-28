// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

#include "ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Adding num slots %d"), AttachmentSlots.Num());
	for (int i = 0; i < AttachmentSlots.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding slot %d %s"), i, *AttachmentSlots[i].ToString());

		AttachmentItems.Add(AttachmentSlots[i]);
	}
}

bool UInventoryComponent::CanAddIntoCargo(AItemBase* item, FIntVector2 pos)
{
	if (item == NULL)
		return false;

	return true;
}

bool UInventoryComponent::AddIntoCargo(AItemBase* item, FIntVector2 pos)
{
	if (!CanAddIntoCargo(item, pos))
		return false;

	FInventoryLocation src_loc = item->Location;
	FInventoryLocation dst_loc = FInventoryLocation::Cargo(this, pos);

	CargoItems.Add(item);

	item->Location = dst_loc;

	if (src_loc.Owning != NULL)
	{
		if (!src_loc.AttachmentSlot.IsNone())
		{
			src_loc.Owning->OnItemDetached.Broadcast(item);
		}
		else
		{
			src_loc.Owning->OnItemRemovedFromCargo.Broadcast(item);
		}
	}

	OnItemAddedToCargo.Broadcast(item);

	return true;
}

AItemBase* UInventoryComponent::GetCargo(FIntVector2 pos)
{
	return nullptr;
}

bool UInventoryComponent::CanSetAttachment(AItemBase* item, FName attachmentSlot)
{
	if (item == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CanSetAttachment - Fail 1"));
		return false;
	}

	if (!AttachmentItems.Contains(attachmentSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CanSetAttachment - Fail 2"));
		return false;
	}
	
	if (AttachmentItems[attachmentSlot] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CanSetAttachment - Fail 3"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CanSetAttachment - Success"));

	return true;
}

bool UInventoryComponent::SetAttachment(AItemBase* item, FName attachmentSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetAttachment"));
	if (!CanSetAttachment(item, attachmentSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetAttachment - Fail"));
		return false;
	}

	FInventoryLocation src_loc = item->Location;
	FInventoryLocation dst_loc = FInventoryLocation::Attachment(this, attachmentSlot);

	AttachmentItems[attachmentSlot] = item;

	item->Location = dst_loc;

	if (src_loc.Owning != NULL)
	{
		if (!src_loc.AttachmentSlot.IsNone())
		{
			src_loc.Owning->OnItemDetached.Broadcast(item);
		}
		else
		{
			src_loc.Owning->OnItemRemovedFromCargo.Broadcast(item);
		}
	}

	OnItemAttached.Broadcast(item);

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetAttachment - Success"));

	return true;
}

AItemBase* UInventoryComponent::GetAttachment(FName attachmentSlot)
{
	if (!AttachmentItems.Contains(attachmentSlot))
		return nullptr;

	return AttachmentItems[attachmentSlot];
}

void UInventoryComponent::MapCargo(TArray<bool>& map, AItemBase* exclude)
{
	map.Reserve(CargoSize.X * CargoSize.Y);

	for (int i = 0; i < CargoItems.Num(); i++)
	{
		AItemBase* item = CargoItems[i];
		if (!item || item == exclude)
			continue;
	
		int w = item->Size.X;
		int h = item->Size.Y;
		int row = item->Location.CargoPosition.X;
		int col = item->Location.CargoPosition.Y;
	
		for (int i_row = row; i_row < row + h; i_row++)
		{
			for (int i_col = col; i_col < col + w; i_col++)
			{
				map[CargoSize.X * i_row + i_col] = true;
			}
		}
	}
}

int UInventoryComponent::ItemFitsInCargo(TArray<bool> map, AItemBase * item)
{
	return 0;
}
