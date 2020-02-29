// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "VT_InventoryLocation.h"

#include "VT_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedToCargo, class AVT_ItemBase*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedFromCargo, class AVT_ItemBase*, item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAttached, class AVT_ItemBase*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDetached, class AVT_ItemBase*, item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLETEST_API UVT_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cargo, meta = (AllowPrivateAccess = "true"))
	TArray<class AVT_ItemBase*> CargoItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attachment, meta = (AllowPrivateAccess = "true"))
	TMap<FName, class AVT_ItemBase*> AttachmentItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attachment, meta = (AllowPrivateAccess = "true"))
	TArray<FName> AttachmentSlots;
public:	
	// Sets default values for this component's properties
	UVT_InventoryComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cargo)
	FIntVector2 CargoSize;

	UFUNCTION(BlueprintCallable)
	bool CanAddIntoCargo(class AVT_ItemBase* item, FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	bool AddIntoCargo(class AVT_ItemBase* item, FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	class AVT_ItemBase* GetCargo(FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	bool CanSetAttachment(class AVT_ItemBase* item, FName attachmentSlot);

	UFUNCTION(BlueprintCallable)
	bool SetAttachment(class AVT_ItemBase* item, FName attachmentSlot);

	UFUNCTION(BlueprintCallable)
	class AVT_ItemBase* GetAttachment(FName attachmentSlot);
private:
	UFUNCTION()
	void MapCargo(TArray<bool> &map, class AVT_ItemBase* exclude);

	UFUNCTION()
	int ItemFitsInCargo(TArray<bool> map, class AVT_ItemBase* item);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;		

public:
	UPROPERTY(BlueprintAssignable, Category = Cargo)
	FItemAddedToCargo OnItemAddedToCargo;

	UPROPERTY(BlueprintAssignable, Category = Cargo)
	FItemRemovedFromCargo OnItemRemovedFromCargo;

	UPROPERTY(BlueprintAssignable, Category = Attachment)
	FItemAttached OnItemAttached;

	UPROPERTY(BlueprintAssignable, Category = Attachment)
	FItemDetached OnItemDetached;
};
