// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class APlayerCharacter;
class UInteractionComponent;

UCLASS()
class SUMMERPROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	
	UPROPERTY(EditAnywhere);
	TObjectPtr<USceneComponent> Root = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actor Component|Interaction");
	TObjectPtr<UInteractionComponent> InteractionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void PickedUp(APlayerCharacter* Player);
	void Dropped();
	
	APlayerCharacter* OwnerPlayer = nullptr;

	void SimulatePhysic(bool simulate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SimulatePhysic(bool simulate);
};
