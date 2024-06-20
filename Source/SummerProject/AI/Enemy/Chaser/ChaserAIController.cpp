// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserAIController.h"

#include "ChaserAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "SummerProject/Player/PlayerCharacter.h"


// Sets default values
AChaserAIController::AChaserAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetupPerceptionSystem();
}

// Called when the game starts or when spawned
void AChaserAIController::BeginPlay()
{
	Super::BeginPlay();
	if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("test"));}
	
}

void AChaserAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(AChaserAICharacter* const ChaserCharacter = Cast<AChaserAICharacter>(InPawn))
	{
		if(UBehaviorTree* const BehaviorTree = ChaserCharacter->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(BehaviorTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

// Called every frame
void AChaserAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChaserAIController::SetupPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightConfig)
	{
		SetPerceptionComponent(*AIPerception);

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AChaserAIController::OnTargetDetected);
		
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
	
}

void AChaserAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	
	
	if(auto const Subject = Cast<APlayerCharacter>(Actor))
	{
		Blackboard->SetValueAsBool("SeePlayer", Stimulus.WasSuccessfullySensed());
		Blackboard->SetValueAsObject("TargetPlayer", Subject);

		if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("See"));}
	}
	else
	{
		Blackboard->SetValueAsBool("SeePlayer", false);
		if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not See"));}
	}
	
	
	/*if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if(auto const Subject = Cast<APlayerCharacter>(Actor))
		{
			Blackboard->SetValueAsBool("SeePlayer", Stimulus.WasSuccessfullySensed());
		}
	}*/
}

