// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "SummerProject/ActorComponents/HealthComponent/HealthComponent.h"
#include "SummerProject/ActorComponents/InventoryComponent/InventoryComponent.h"
#include "PlayerCharacter.generated.h"


class ISubmarineControl;
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
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ItemPlaceholder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;
	
	
	
	UInteractionComponent* InteractionLineTrace(int16 TraceDistance);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionComponent* LookingAtInteractionComponent = nullptr;
	
	void UseInteractable(UInteractionComponent* Component);
	UFUNCTION(Server, Reliable)
	void Server_UseInteractable(UInteractionComponent* Component);
	
	void UseSubmarineController(ISubmarineControl* ControlInterface);
	void StopUseSubmarineController();
protected:
	
	UPROPERTY(BlueprintReadWrite)
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
	void HandleStopRun();
	void HandleUse();
	void HandleStopUse();
	void HandleDropItem();

	ISubmarineControl* SubmarineControlInterface = nullptr;
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UnlitMode();

public:
	UFUNCTION(BlueprintCallable)
	void ResourceCalculation(float DeltaTime);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Status")
	bool bIsInWater = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Oxygen")
	float MaxOxygen = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Oxygen")
	float Oxygen = MaxOxygen;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Oxygen")
	float OxygenDepletionRate = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Stamina")
	float Stamina = MaxStamina;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Stamina")
	float StaminaDepletionRate = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Stamina")
	float StaminaIncreaseRate = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Resource|Stamina")
	float StaminaResourceDepletionScale = 1.0f;

	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Movement")
	bool bIsRunning = false;
};
