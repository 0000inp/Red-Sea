// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "DefaultPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class APlayerCharacter;


UCLASS(Abstract)
class SUMMERPROJECT_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;;
	
	//Input mapping context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
	
	//IA movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionMove = nullptr;
	
	//IA Look
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionLook = nullptr;
	
	//IA Jump
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionJump = nullptr;

	//IA Run
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionRun = nullptr;
	
	
	//input hande function
	void HandleMove(const FInputActionValue& IAVal);
	void HandleLook(const FInputActionValue& IAVal);
	void HandleJump();
	void HandleRun();
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	
	
	
private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	//ref to possing Pawn
	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	
};
