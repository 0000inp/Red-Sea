#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "JsonUtilities.h"
#include "Json.h"
#include "GeminiAPI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResponseReceived, FString, ResponseText);

UCLASS(Blueprintable)
class SUMMERPROJECT_API AGeminiAPI : public AActor
{
	GENERATED_BODY()

public:
	
	TSharedPtr<FJsonObject> ReadJSON(FString FilePath, bool& bOutSuccess);
	FString ReadStringFromFile(FString FilePath, bool& bOutSuccess);
	
	// Send prompt to Gemini AP
	UFUNCTION(BlueprintCallable, Category = "Gemini API")
	void SendPrompt(const FString& Prompt);
	
	// Callback for when the API response is received
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnResponseReceived OnResponseReceivedEvent;

	//Prompt Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Config")
	float Temperature = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Config")
	float TopK = 64;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Config")
	float TopP = 0.95;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Config")
	int MaxOutputTokens = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Config")
	FString ResponseMimeType = "text/plain";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemini API")
	FString APIKey = TEXT("AIzaSyAFcBzUx_MfqqERIHTgbGYvP9KKJAYaVrw");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemini API")
	FString APIEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=AIzaSyAFcBzUx_MfqqERIHTgbGYvP9KKJAYaVrw");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facts")
	FString Facts = "in this savior-687 planet there is a operation from human, every one in this facility is working for star sailor hope. they mine resource and oil under water. they has gone insane because of some wave from the core of the planet. and start going deeper in the ocean to find the core then they all disappear";
};
