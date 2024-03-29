// Copyright Epic Games, Inc. All Rights Reserved.


#include "SummerProjectGameModeBase.h"
#include "GameFramework/PlayerController.h"

void ASummerProjectGameModeBase::PreLogin(const FString& Options, const FString& Address,
	const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString ASummerProjectGameModeBase::InitNewPlayer(APlayerController* NewPlayerController,
	const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	// UE_LOG(LogTemp, Warning, TEXT("-------From GameModeBase-------"))
	// if(NewPlayerController){UE_LOG(LogTemp, Warning, TEXT("Controller = %s"), *NewPlayerController->GetName())} else{UE_LOG(LogTemp, Warning, TEXT("No Controller"))}
	// if(NewPlayerController->InputComponent){UE_LOG(LogTemp, Warning, TEXT("InputComponent = %s"), *NewPlayerController->InputComponent->GetName())} else{UE_LOG(LogTemp, Warning, TEXT("No InputComponent"))}
	// if(NewPlayerController->Player){UE_LOG(LogTemp, Warning, TEXT("Player = %s"), *NewPlayerController->Player->GetName())} else{UE_LOG(LogTemp, Warning, TEXT("No Player"))}
	UE_LOG(LogTemp,Warning,TEXT("---------%s-------->>>>>------- is local controller = %hs"),*GetName(), NewPlayerController->IsLocalController() ? "true" : "false")
	// UE_LOG(LogTemp, Warning, TEXT("--------------"))
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}
