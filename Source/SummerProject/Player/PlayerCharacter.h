// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ADefaultPlayerController;
class UInputAction;
class UInputMappingContext;
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
	
protected:

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	//input hande function
	void HandleMove(const FInputActionValue& IAVal);
	void HandleLook(const FInputActionValue& IAVal);
	void HandleJump();
	void HandleRun();

	ADefaultPlayerController* DefaultPlayerController;
};
