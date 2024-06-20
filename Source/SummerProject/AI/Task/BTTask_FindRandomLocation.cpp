// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SummerProject/AI/Enemy/Chaser/ChaserAICharacter.h"
#include "SummerProject/AI/Enemy/Chaser/ChaserAIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get ai controller and character
	if(AChaserAIController* const Controller = Cast<AChaserAIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const AICharacter = Controller->GetPawn())
		{
			//get ai origin
			FVector const Origin = AICharacter->GetActorLocation();
			
			//get navigation system and find random location
			if(auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if(NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					//set value in BlackBoard
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}
				
				//finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
