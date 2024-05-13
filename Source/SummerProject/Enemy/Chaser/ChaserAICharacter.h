// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "ChaserAICharacter.generated.h"

UCLASS()
class SUMMERPROJECT_API AChaserAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChaserAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAIPerceptionComponent* Perception;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
