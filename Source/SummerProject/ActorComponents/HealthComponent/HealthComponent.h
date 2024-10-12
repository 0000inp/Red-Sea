// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, DamageAmount);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SUMMERPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Health")
	float MaxHealthPoint = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Health")
	float HealthPoint = MaxHealthPoint;
	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnTakeDamage OnTakeDamage;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateHealthPoint(float value);
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(float Damage);
	void Heal(float HealPoint);
	void Dead();
};
