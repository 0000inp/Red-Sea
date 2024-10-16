// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindSubmarineLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SummerProject/Dev/DEBUG.h"
#include "SummerProject/Player/Submarine/Submarine.h"
class AChaserAIController;

UBTTask_FindSubmarineLocation::UBTTask_FindSubmarineLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Submarine Location");
}

EBTNodeResult::Type UBTTask_FindSubmarineLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(auto* const Submarine = Cast<ASubmarine>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetSubmarine")))
	{
		
		FVector OutputLocaion = Submarine->GetActorLocation();;

		if (bGetSurfaceLocation == false)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), OutputLocaion);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		
		FVector StartLcation = OwnerComp.GetOwner()->GetActorLocation();
		FVector EndLocation = Submarine->GetActorLocation();

		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, StartLcation, EndLocation, ECC_GameTraceChannel4))
		{
			OutputLocaion = Hit.Location;
		}
		else{return EBTNodeResult::Failed;}

		//DEBUG::print(OutputLocaion.ToString(),5.0f,FColor::Purple);
		DrawDebugPoint(GetWorld(), OutputLocaion, 10.0f, FColor::Purple, false, 0.1f);
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), OutputLocaion);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
