// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SummerProject/AI/Enemy/Chaser/ChaserAIController.h"

UBTTask_Chase::UBTTask_Chase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(auto* const AIController = Cast<AChaserAIController>(OwnerComp.GetAIOwner()))
	{
		//auto const ChaseLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
