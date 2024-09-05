// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveTo3D.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERPROJECT_API UBTTask_MoveTo3D : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_MoveTo3D(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DistanceTolerance = 60;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed = 5;
	
	
};
