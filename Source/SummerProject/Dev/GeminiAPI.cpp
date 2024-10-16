#include "GeminiAPI.h"

#include "DEBUG.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"

void AGeminiAPI::SendPrompt(const FString& Prompt)
{
	// Set up the Http request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AGeminiAPI::OnResponseReceived);
	Request->SetURL(APIEndpoint);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	//Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));

	// Create JSON request body
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
	// Create the "parts" object with the provided prompt text
	TArray<TSharedPtr<FJsonValue>> PartsArray;
	TSharedPtr<FJsonObject> PartObject = MakeShareable(new FJsonObject);
	PartObject->SetStringField("text", Prompt);
	PartsArray.Add(MakeShareable(new FJsonValueObject(PartObject)));
    
	// Add the parts array to the "contents" array
	TArray<TSharedPtr<FJsonValue>> ContentsArray;
	TSharedPtr<FJsonObject> ContentsObject = MakeShareable(new FJsonObject);
	ContentsObject->SetArrayField("parts", PartsArray);
	ContentsArray.Add(MakeShareable(new FJsonValueObject(ContentsObject)));
    
	// Add the contents array to the main JSON object
	JsonObject->SetArrayField("contents", ContentsArray);

	// GenerationConfig part
	/*TSharedPtr<FJsonObject> GenerationConfigObject = MakeShareable(new FJsonObject);
	GenerationConfigObject->SetNumberField("temperature", Temperature);
	GenerationConfigObject->SetNumberField("topK", TopK);
	GenerationConfigObject->SetNumberField("topP", TopP);
	GenerationConfigObject->SetNumberField("maxOutputTokens", MaxOutputTokens);
	GenerationConfigObject->SetStringField("responseMimeType", ResponseMimeType);
	
	JsonObject->SetObjectField("generationConfig", GenerationConfigObject);*/
	
	// Convert JSON to string and attach it to the request
	FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void AGeminiAPI::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Request failed"));
		return;
	}
	
	// Parse the JSON response
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// Get the "candidates" array
		const TArray<TSharedPtr<FJsonValue>>* Candidates;
		if (JsonObject->TryGetArrayField("candidates", Candidates))
		{
			// Loop through candidates and extract the "text" field from the "parts" array
			for (const TSharedPtr<FJsonValue>& Candidate : *Candidates)
			{
				const TSharedPtr<FJsonObject> CandidateObject = Candidate->AsObject();
				const TSharedPtr<FJsonObject> ContentObject = CandidateObject->GetObjectField("content");

				const TArray<TSharedPtr<FJsonValue>>* PartsArray;
				if (ContentObject->TryGetArrayField("parts", PartsArray))
				{
					for (const TSharedPtr<FJsonValue>& Part : *PartsArray)
					{
						FString Text = Part->AsObject()->GetStringField("text");
						UE_LOG(LogTemp, Log, TEXT("Response Text: %s"), *Text);
						DEBUG::print("Gemini Reply : ",10.0f,FColor::Purple);
						DEBUG::print(Text,10.0f,FColor::Purple);
						OnResponseReceivedEvent.Broadcast(Text);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
		DEBUG::print("Failed to parse JSON response",10.0f,FColor::Red);
	}
}
