// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ChaserAIController.generated.h"

class UAISenseConfig;
struct FAIStimulus;

UCLASS()
class SUMMERPROJECT_API AChaserAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChaserAIController();
	
	TObjectPtr<UAIPerceptionComponent> AIPerception;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UAISenseConfig* SightConfig;
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	
};
