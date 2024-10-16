// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthPoint = MaxHealthPoint;
	
}

void UHealthComponent::TakeDamage(float Damage)
{
	if (Damage < DamageThreshold)
	{
		return;
	}
	
	UpdateHealthPoint(-1 * Damage);
	
	OnTakeDamage.Broadcast(Damage);
}

void UHealthComponent::Heal(float HealPoint)
{
	UpdateHealthPoint(HealPoint);
}

void UHealthComponent::UpdateHealthPoint(float value)
{
	HealthPoint += value;
	HealthPoint = FMath::Clamp(HealthPoint,0.0f,MaxHealthPoint);
}

void UHealthComponent::Dead()
{
	GetOwner()->Destroy();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

