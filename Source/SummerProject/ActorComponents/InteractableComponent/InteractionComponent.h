// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class APlayerCharacter;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsedSignature,APlayerCharacter*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopUsedSignature,APlayerCharacter*, Player);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SUMMERPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction");
	FOnUsedSignature onInteract;

	UPROPERTY(BlueprintAssignable, Category = "Interaction");
	FOnStopUsedSignature onStopInteract;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interaction");
	bool HoldMode = false;

	bool Holding = false;
	
	UFUNCTION()
	virtual void Interact(APlayerCharacter* Player);

	UFUNCTION()
	virtual void StopInteract(APlayerCharacter* Player);
	
	//virtual void Drag(APawn* Pawn, FVector Effector);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

