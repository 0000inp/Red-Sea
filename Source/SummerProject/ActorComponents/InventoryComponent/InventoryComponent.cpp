// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "SummerProject/Item/Item.h"


class AItem;
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(AItem* Item, uint8 index)
{
	if(Inv[index] == nullptr)
	{
		Inv[index] = Item;
	}
}

AItem* UInventoryComponent::RemoveItem(uint8 index)
{
	if(Inv[index] != nullptr)
	{
		AItem* ItemToRemove = Inv[index];
		Inv[index] = nullptr;
		return ItemToRemove;
	}
	return nullptr;
}

AItem* UInventoryComponent::GetHoldingItem(){return HoldingItem;}

void UInventoryComponent::SetHoldingItem(AItem* Item){HoldingItem = Item;}

void UInventoryComponent::DropItem()
{
	AActor* Owner = GetOwner();
	if(Owner && Owner->HasAuthority())
	{
		if(HoldingItem)
		{
			HoldingItem->Dropped();
			SetHoldingItem(nullptr);
		}
	}
	else{Server_DropItem();}
}

void UInventoryComponent::Server_DropItem_Implementation(){DropItem();}



