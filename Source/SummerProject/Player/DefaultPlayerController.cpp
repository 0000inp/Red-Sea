// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void ADefaultPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
<<<<<<< HEAD

	//store ref to player char if possesing pawn in player character
	Avatar = Cast<APlayerCharacter>(aPawn);
	checkf(Avatar, TEXT("Need to possess APlayerCharacter type"));
	
	//get ref to EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(Avatar->CharacterInputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get ref to EnhancedInputComponent"));
	
	//get local player subsystem
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Avatar->LocalPlayer);
	checkf(InputSubsystem, TEXT("Unable to get ref to EnhancedInputLocalPlayerSubsystem"));
	checkf(InputMappingContext, TEXT("InputMappingContext is not assigned"));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
=======
	
	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("Need to possess APlayerCharacter type"));
	
	/**
>>>>>>> recover
	
	
	//bind input action
	if(ActionMove){EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleMove);}
	if(ActionLook){EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleLook);}
	if(ActionJump){EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleJump);}
	if(ActionRun){EnhancedInputComponent->BindAction(ActionRun, ETriggerEvent::Triggered, this, &ADefaultPlayerController::HandleRun);}
	
}

void ADefaultPlayerController::OnUnPossess()
{
	//Unbind all Actions
	EnhancedInputComponent->ClearActionBindings();
	
	Super::OnUnPossess();
}

void ADefaultPlayerController::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2d MovementVector = IAVal.Get<FVector2d>();
	Avatar->AddMovementInput(Avatar->GetActorForwardVector(), MovementVector.Y);
	Avatar->AddMovementInput(Avatar->GetActorRightVector(), MovementVector.X);
}

void ADefaultPlayerController::HandleLook(const FInputActionValue& IAVal)
{
	const FVector2d LookVector = IAVal.Get<FVector2d>();
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ADefaultPlayerController::HandleJump()
{
	Avatar->Jump();
}

void ADefaultPlayerController::HandleRun()
{
	printf("RUN");
}





