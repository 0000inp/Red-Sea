#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SummerProject/AI/Enemy/Chaser/ChaserAIController.h"
#include "SummerProject/Interface/IAIActions.h"

UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IAIActions* ControlledPawn = Cast<IAIActions>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		ControlledPawn->Attack();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}