// Fill out your copyright notice in the Description page of Project Settings.


#include "FishAIController.h"
#include "FishAIPawn.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "SummerProject/Dev/DEBUG.h"
#include "SummerProject/Player/PlayerCharacter.h"


// Sets default values
AFishAIController::AFishAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetupPerceptionSystem();
}

// Called when the game starts or when spawned
void AFishAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AFishAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(AFishAIPawn* const FPawn = Cast<AFishAIPawn>(InPawn))
	{
		FishPawn = FPawn;
		if(UBehaviorTree* const BehaviorTree = FishPawn->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(BehaviorTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(BehaviorTree);
		}
	}
}

// Called every frame
void AFishAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFishAIController::SetupPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightConfig)
	{
		SetPerceptionComponent(*AIPerception);

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AFishAIController::OnTargetDetected);
		
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AFishAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	
}

void AFishAIController::FindPathToTarget(FVector TargetLocation)
{
	if (!CPathVolume){return;}
	auto FunctionName = GET_FUNCTION_NAME_CHECKED(AFishAIController, OnPathFound);
	//CPathVolume->FindPathAsync(this, FunctionName, FishPawn->GetActorLocation(), TargetLocation);
	Path = CPathVolume->FindPathSynchronous(FishPawn->GetActorLocation(), TargetLocation).UserPath;
}

void AFishAIController::OnPathFound(FCPathResult& PathResult)
{
	Path = PathResult.UserPath;
	CurrentPathIndex = 1;
}

void AFishAIController::MoveTo(FVector Location, float DistanceTolerance)
{
	
	FVector ActorLocation = FishPawn->GetActorLocation();
	
	double Distance = UKismetMathLibrary::Vector_Distance(Location, ActorLocation);
	
	if(Distance > DistanceTolerance)
	{
		/*FishPawn->SetActorRotation(
			UKismetMathLibrary::RInterpTo(
				FishPawn->GetActorRotation(),
				UKismetMathLibrary::FindLookAtRotation(ActorLocation, Location),
				0.02,
				5.0)
		);*/
		
		FVector InputVector = (Location - ActorLocation).GetSafeNormal();
		
		
		CurrentInputVector = InputVector;
		
		//FishPawn->GetMovementComponent()->AddInputVector(InputVector);
	}
	else
	{
		//CurrentInputVector = FVector::ZeroVector;
		CurrentPathIndex += 1;
	}
}


void AFishAIController::UpdatePath()
{
	FindPathToTarget(GetBlackboardComponent()->GetValueAsVector(FName(TEXT("TargetLocation"))));
}

void AFishAIController::RotateToVelocity(float DeltaTime, float RotationRate)
{
	FVector PawnVelocity = FishPawn->FloatingPawnMovementComponent->Velocity;

	if (!PawnVelocity.IsNearlyZero())
	{
		FRotator TargetRotation = PawnVelocity.Rotation();
		FRotator NewRotation = FMath::RInterpTo(
			FishPawn->GetActorRotation(),
			TargetRotation,
			GetWorld()->GetDeltaSeconds(),
			RotationRate * DeltaTime
		);
		
		FishPawn->SetActorRotation(NewRotation);
	}
}


