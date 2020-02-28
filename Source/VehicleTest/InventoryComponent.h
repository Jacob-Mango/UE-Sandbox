// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventoryLocation.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedToCargo, class AItemBase*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedFromCargo, class AItemBase*, item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAttached, class AItemBase*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDetached, class AItemBase*, item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLETEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cargo, meta = (AllowPrivateAccess = "true"))
	TArray<class AItemBase*> CargoItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attachment, meta = (AllowPrivateAccess = "true"))
	TMap<FName, class AItemBase*> AttachmentItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attachment, meta = (AllowPrivateAccess = "true"))
	TArray<FName> AttachmentSlots;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cargo)
	FIntVector2 CargoSize;

	UFUNCTION(BlueprintCallable)
	bool CanAddIntoCargo(class AItemBase* item, FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	bool AddIntoCargo(class AItemBase* item, FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	class AItemBase* GetCargo(FIntVector2 pos);

	UFUNCTION(BlueprintCallable)
	bool CanSetAttachment(class AItemBase* item, FName attachmentSlot);

	UFUNCTION(BlueprintCallable)
	bool SetAttachment(class AItemBase* item, FName attachmentSlot);

	UFUNCTION(BlueprintCallable)
	class AItemBase* GetAttachment(FName attachmentSlot);
private:
	UFUNCTION()
	void MapCargo(TArray<bool> &map, class AItemBase* exclude);

	UFUNCTION()
	int ItemFitsInCargo(TArray<bool> map, class AItemBase* item);
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
