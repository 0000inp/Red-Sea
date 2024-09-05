// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveTo3D.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SummerProject/AI/Enemy/Chaser/ChaserAIController.h"
#include "SummerProject/AI/Enemy/Fish/FishAIPawn.h"
#include "SummerProject/AI/Enemy/Fish/FishAIController.h"
#include "SummerProject/Dev/DEBUG.h"

UBTTask_MoveTo3D::UBTTask_MoveTo3D(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("MoveTo3D");
}

EBTNodeResult::Type UBTTask_MoveTo3D::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(auto* const AIController = Cast<AFishAIController>(OwnerComp.GetAIOwner()))
	{
		if(APawn* Pawn = Cast<APawn>(AIController->GetPawn()))
		{
			FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			FVector ActorLocation = Pawn->GetActorLocation();
			
			double Distance = UKismetMathLibrary::Vector_Distance(TargetLocation, ActorLocation);
			
			if(Distance > DistanceTolerance)
			{
				Pawn->SetActorRotation(
					UKismetMathLibrary::RInterpTo(
						Pawn->GetActorRotation(),
						UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation),
						0.02,
						5.0)
				);
				
				//DEBUG::print("MOVETO3d");
				
				FVector CurrentInputVector = (TargetLocation - ActorLocation);
				CurrentInputVector.Normalize(0.0001);
				CurrentInputVector *= Speed;
				
				FName InputVectorKey = FName(TEXT("MovementInputVector"));
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(InputVectorKey, CurrentInputVector);
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
	

