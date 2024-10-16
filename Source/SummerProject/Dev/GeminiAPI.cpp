#include "GeminiAPI.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"

void UGeminiAPI::SendPrompt(const FString& Prompt)
{
	// Set up the Http request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGeminiAPI::OnResponseReceived);
	Request->SetURL(APIEndpoint);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));

	// Create the JSON body for the request
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("prompt", Prompt);

	// Convert JSON to string and attach it to the request
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void UGeminiAPI::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Request failed"));
		return;
	}

	// Parse the JSON response
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// Extract the response text
		FString Reply = JsonObject->GetStringField("choices");
		UE_LOG(LogTemp, Log, TEXT("Gemini API Reply: %s"), *Reply);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
	}
}
