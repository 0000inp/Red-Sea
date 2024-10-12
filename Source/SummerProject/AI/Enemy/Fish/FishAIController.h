// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FishAIPawn.h"
#include "CPathfinding/Public/CPathVolume.h"

#include "FishAIController.generated.h"

class UAISenseConfig;
struct FAIStimulus;

UCLASS()
class SUMMERPROJECT_API AFishAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishAIController();
	
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACPathVolume* CPathVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(BlueprintReadWrite)
	int CurrentPathIndex;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCPathNode> Path;
	
	UFUNCTION(BlueprintCallable)
	void FindPathToTarget(FVector TargetLocation);

	UFUNCTION()
	void OnPathFound(FCPathResult& PathResult);
	
	UFUNCTION(BlueprintCallable)
	void MoveTo(FVector Location, float DistanceTolerance = 60);

	UFUNCTION(BlueprintCallable)
	void RotateToVelocity(float DeltaTime, float RotationRate);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurrentInputVector;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AFishAIPawn* FishPawn;

	UFUNCTION(BlueprintCallable)
	void UpdatePath();
private:
	UAISenseConfig* SightConfig;
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	
};
