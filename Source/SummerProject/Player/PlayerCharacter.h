// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "SummerProject/ActorComponents/HealthComponent/HealthComponent.h"
#include "SummerProject/ActorComponents/InventoryComponent/InventoryComponent.h"
#include "PlayerCharacter.generated.h"


class UInteractionComponent;
class UCharacterMovementComponent;
class UCameraComponent;
class ADefaultPlayerController;
struct FInputActionValue;
class APlayerCharacter;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class SUMMERPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ItemPlaceholder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

public:
	
	void InteractionLineTrace(int16 TraceDistance);
	UFUNCTION(Server, Reliable)
	void Server_UseInteractable(UInteractionComponent* Component);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<ADefaultPlayerController> DefaultPlayerController = nullptr;
	
	UPROPERTY(EditAnywhere)
	int16 InteractionRange = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;
	void SetupStimulusSource();

	//input hande function
	void HandleMove(const FInputActionValue& IAVal);
	void HandleLook(const FInputActionValue& IAVal);
	void HandleJump();
	void HandleRun();
	void HandleUse();
	void HandleDropItem();
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UnlitMode();
};
