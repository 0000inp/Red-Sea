// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Perception/AIPerceptionTypes.h"
#include "SummerProject/Interface/IAIActions.h"
#include "FishAIPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttack, float, DamageAmount);

class UBehaviorTree;

UCLASS()
class SUMMERPROJECT_API AFishAIPawn : public APawn, public IAIActions
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay|Attack")
	float AttackRange = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay|Attack")
	float AttackRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay|Attack")
	float AttackDamage = 40.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovementComponent = nullptr;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UBehaviorTree* GetBehaviorTree() const;

	//Actions
	
	UFUNCTION(BlueprintCallable, Blueprintable, BlueprintNativeEvent)
	void Attack() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAttack OnAttack;
	
};
