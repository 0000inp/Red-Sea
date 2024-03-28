// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
class UCameraComponent;
class ADefaultPlayerController;
struct FInputActionValue;
class APlayerCharacter;

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

	UPROPERTY()
	UInputComponent* CharacterInputComponent = nullptr;
	UPROPERTY()
	ULocalPlayer* LocalPlayer = nullptr;
	
	
	void InteractionLineTrace(int16 TraceDistance);
	
protected:

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
<<<<<<< HEAD
=======
	//input hande function
	void HandleMove(const FInputActionValue& IAVal);
	void HandleLook(const FInputActionValue& IAVal);
	void HandleJump();
	void HandleRun();
	void HandleUse();

	UPROPERTY()
	ADefaultPlayerController* DefaultPlayerController;

	UPROPERTY(EditAnywhere)
	int16 InteractionRange = 250.0f;
>>>>>>> recover
};
