// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// UE_LOG(LogTemp, Warning, TEXT("-------From %s-------"),*GetName())
	// if(InputComponent){UE_LOG(LogTemp, Warning, TEXT("InputComponent = %s-------"),*InputComponent->GetName())}
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
	//Avatar->EnhancedInputComponent->ClearActionBindings();
	
	Super::OnUnPossess();
}






