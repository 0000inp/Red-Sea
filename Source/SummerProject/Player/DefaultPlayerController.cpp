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
	
	//store ref to player char if possesing pawn in player character
	Avatar = Cast<APlayerCharacter>(aPawn);
	checkf(Avatar, TEXT("Need to possess APlayerCharacter type"));
	
}

void ADefaultPlayerController::OnUnPossess()
{
	//Unbind all Actions
	EnhancedInputComponent->ClearActionBindings();
	
	Super::OnUnPossess();
}






