#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "JsonUtilities.h"
#include "Json.h"
#include "GeminiAPI.generated.h"

UCLASS(Blueprintable)
class SUMMERPROJECT_API UGeminiAPI : public UObject
{
	GENERATED_BODY()

public:
	// Send prompt to Gemini API
	UFUNCTION(BlueprintCallable, Category = "Gemini API")
	void SendPrompt(const FString& Prompt);
	
	// Callback for when the API response is received
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemini API")
	FString APIEndpoint = TEXT("https://api.gemini.ai/v1/chat");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemini API")
	FString APIKey = TEXT("your_api_key_here");
};
