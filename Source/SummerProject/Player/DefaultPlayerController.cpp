// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}


void ADefaultPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("Need to possess APlayerCharacter type"));
	
	UInputComponent* IC = InputComponent;
	ULocalPlayer* player = GetLocalPlayer();
	
	/**
	//store ref to player char
	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("Need to possess APlayerCharacter type"));
	
	SetupInputComponent();
	
	//get ref to EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	checkf(EnhancedInputComponent, TEXT("Unable to get ref to EnhancedInputComponent"));	
	
	//get local player subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	checkf(InputSubsystem, TEXT("Unable to get ref to EnhancedInputLocalPlayerSubsystem"));

	checkf(InputMappingContext, TEXT("InputMappingContext is not assigned"));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
	//bind input action
	if(ActionMove){EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleMove);}
	if(ActionLook){EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleLook);}
	if(ActionJump){EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleJump);}
	if(ActionRun){EnhancedInputComponent->BindAction(ActionRun, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleRun);}
	**/
}

void ADefaultPlayerController::OnUnPossess()
{
	//Unbind all Actions
	//EnhancedInputComponent->ClearActionBindings();
	
	Super::OnUnPossess();
}

void ADefaultPlayerController::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2d MovementVector = IAVal.Get<FVector2d>();
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
}

void ADefaultPlayerController::HandleLook(const FInputActionValue& IAVal)
{
	const FVector2d LookVector = IAVal.Get<FVector2d>();
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ADefaultPlayerController::HandleJump()
{
	PlayerCharacter->Jump();
}

void ADefaultPlayerController::HandleRun()
{
	printf("RUN");
}




