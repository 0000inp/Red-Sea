#include "GeminiAPI.h"

#include "DEBUG.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"

FString AGeminiAPI::ReadStringFromFile(FString FilePath, bool& bOutSuccess)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		bOutSuccess = false;
		return "File does not exist";
	}
	FString Content = "";
	if (!FFileHelper::LoadFileToString(Content, *FilePath))
	{
		bOutSuccess = false;
		return "File could not be read";
	}
	bOutSuccess = true;
	return Content;
}

TSharedPtr<FJsonObject> AGeminiAPI::ReadJSON(FString FilePath, bool& bOutSuccess)
{
	FString Content = ReadStringFromFile(FilePath, bOutSuccess);
	if (!bOutSuccess){return nullptr;}

	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Content), JsonObject))
	{
		bOutSuccess = false;
		return nullptr;
	}

	bOutSuccess = true;
	return JsonObject;
}


void AGeminiAPI::SendPrompt(const FString& Prompt)
{
	
	// Set up the Http request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AGeminiAPI::OnResponseReceived);
	Request->SetURL(APIEndpoint);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	//Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));

	/*
	// Create JSON request body
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	//INFO
	TSharedPtr<FJsonObject> FactsChatPrompt = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> FactPartsArray;
	TSharedPtr<FJsonObject> FactsPartObject = MakeShareable(new FJsonObject);
	FactsChatPrompt->SetStringField("role", "user");
	FactsPartObject->SetStringField("text", Facts);
	FactPartsArray.Add(MakeShareable(new FJsonValueObject(FactsPartObject)));
	FactsChatPrompt->SetArrayField("parts", FactPartsArray);

	//model Okay
	TSharedPtr<FJsonObject> ModelChatPrompt = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> ModelPartsArray;
	TSharedPtr<FJsonObject> ModelPartObject = MakeShareable(new FJsonObject);
	ModelChatPrompt->SetStringField("role", "model");
	ModelPartObject->SetStringField("text", "Okay. give me facts and i will fact check future prompt in domain of your facts. with answer of only [True, False] no '.' ");
	ModelPartsArray.Add(MakeShareable(new FJsonValueObject(ModelPartObject)));
	ModelChatPrompt->SetArrayField("parts", ModelPartsArray);
    
	// Create the "parts" object with the provided prompt text
	TSharedPtr<FJsonObject> AnswerChatPrompt = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> AnswerPartsArray;
	TSharedPtr<FJsonObject> AnswerPartObject = MakeShareable(new FJsonObject);
	AnswerChatPrompt->SetStringField("role", "user");
	AnswerPartObject->SetStringField("text", Prompt);
	AnswerPartsArray.Add(MakeShareable(new FJsonValueObject(AnswerPartObject)));
	AnswerChatPrompt->SetArrayField("parts", AnswerPartsArray);

	// Add the parts array to the "contents" array
	TArray<TSharedPtr<FJsonValue>> ContentsArray;
	ContentsArray.Add(MakeShareable(new FJsonValueObject(FactsChatPrompt)));
	ContentsArray.Add(MakeShareable(new FJsonValueObject(AnswerChatPrompt)));
    
	// Add the contents array to the main JSON object
	JsonObject->SetArrayField("contents", ContentsArray);
	
	// GenerationConfig part
	/*TSharedPtr<FJsonObject> GenerationConfigObject = MakeShareable(new FJsonObject);
	GenerationConfigObject->SetNumberField("temperature", Temperature);
	GenerationConfigObject->SetNumberField("topK", TopK);
	GenerationConfigObject->SetNumberField("topP", TopP);
	GenerationConfigObject->SetNumberField("maxOutputTokens", MaxOutputTokens);
	GenerationConfigObject->SetStringField("responseMimeType", ResponseMimeType);
	
	JsonObject->SetObjectField("generationConfig", GenerationConfigObject);#1#*/
	
	
	// Convert JSON to string and attach it to the request
	bool bReadSuccess;
	TSharedPtr<FJsonObject> JsonObject = ReadJSON(FPaths::ProjectDir() + TEXT("Source/SummerProject/Dev/PromptBody.json"), bReadSuccess);
	DEBUG::print(bReadSuccess? "read success" : "read fail");
	
	TArray<TSharedPtr<FJsonValue>> ContentsArray = JsonObject->GetArrayField("contents");
	ContentsArray[2]->AsObject()->GetArrayField("parts")[0]->AsObject()->SetStringField("text", Prompt);

	DEBUG::print(ContentsArray[2]->AsObject()->GetArrayField("parts")[0]->AsObject()->GetStringField("text"));
	
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
