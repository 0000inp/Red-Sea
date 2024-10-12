// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class AItem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SUMMERPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	uint8 InvSize = 5;
	
	UPROPERTY()
	AItem* Inv[5];
	
	uint8 HoldingIndex;
	AItem* HoldingItem = nullptr;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(AItem* Item, uint8 index);
	AItem* RemoveItem(uint8 index);
	
	AItem* GetHoldingItem();
	void SetHoldingItem(AItem* Item);

	
	void DropItem();
	UFUNCTION(Server, Reliable)
	void Server_DropItem();
	
	void UseItem(uint8 UseCase);
};
