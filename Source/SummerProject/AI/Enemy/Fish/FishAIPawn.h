// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Perception/AIPerceptionTypes.h"
#include "FishAIPawn.generated.h"

class UBehaviorTree;

UCLASS()
class SUMMERPROJECT_API AFishAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFishAIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovementComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UBehaviorTree* GetBehaviorTree() const;
	
};
