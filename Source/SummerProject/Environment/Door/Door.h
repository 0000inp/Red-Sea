// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UInteractionComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SUMMERPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();
	
	UPROPERTY(EditAnywhere);
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<UStaticMeshComponent> DoorPanel = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<UStaticMeshComponent> DoorFrame = nullptr;

	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<AActor> DoorHinge = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actor Component|Interaction");
	TObjectPtr<UInteractionComponent> InteractionComponent = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnUsed(APawn* Pawn);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
