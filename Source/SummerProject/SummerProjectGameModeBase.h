// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SummerProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERPROJECT_API ASummerProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//ASummerProjectGameModeBase();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
