// Fill out your copyright notice in the Description page of Project Settings.

#include "VT_InventoryComponent.h"

#include "VT_ItemBase.h"

// Sets default values for this component's properties
UVT_InventoryComponent::UVT_InventoryComponent()
{
}

// Called when the game starts
void UVT_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Adding num slots %d"), AttachmentSlots.Num());
	for (int i = 0; i < AttachmentSlots.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding slot %d %s"), i, *AttachmentSlots[i].ToString());

		AttachmentItems.Add(AttachmentSlots[i]);
	}
}

bool UVT_InventoryComponent::CanAddIntoCargo(AVT_ItemBase* item, FIntVector2 pos)
{
	if (item == NULL)
		return false;

	return true;
}

bool UVT_InventoryComponent::AddIntoCargo(AVT_ItemBase* item, FIntVector2 pos)
{
	if (!CanAddIntoCargo(item, pos))
		return false;

	FVT_InventoryLocation src_loc = item->Location;
	FVT_InventoryLocation dst_loc = FVT_InventoryLocation::Cargo(this, pos);

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

AVT_ItemBase* UVT_InventoryComponent::GetCargo(FIntVector2 pos)
{
	return nullptr;
}

bool UVT_InventoryComponent::CanSetAttachment(AVT_ItemBase* item, FName attachmentSlot)
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

bool UVT_InventoryComponent::SetAttachment(AVT_ItemBase* item, FName attachmentSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetAttachment"));
	if (!CanSetAttachment(item, attachmentSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SetAttachment - Fail"));
		return false;
	}

	FVT_InventoryLocation src_loc = item->Location;
	FVT_InventoryLocation dst_loc = FVT_InventoryLocation::Attachment(this, attachmentSlot);

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

AVT_ItemBase* UVT_InventoryComponent::GetAttachment(FName attachmentSlot)
{
	if (!AttachmentItems.Contains(attachmentSlot))
		return nullptr;

	return AttachmentItems[attachmentSlot];
}

void UVT_InventoryComponent::MapCargo(TArray<bool>& map, AVT_ItemBase* exclude)
{
	map.Reserve(CargoSize.X * CargoSize.Y);

	for (int i = 0; i < CargoItems.Num(); i++)
	{
		AVT_ItemBase* item = CargoItems[i];
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

int UVT_InventoryComponent::ItemFitsInCargo(TArray<bool> map, AVT_ItemBase* item)
{
	return 0;
}
