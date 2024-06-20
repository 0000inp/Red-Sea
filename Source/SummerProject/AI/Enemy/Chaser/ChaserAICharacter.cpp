// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserAICharacter.h"


// Sets default values
AChaserAICharacter::AChaserAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AChaserAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChaserAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChaserAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AChaserAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

