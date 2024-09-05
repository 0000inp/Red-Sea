// Fill out your copyright notice in the Description page of Project Settings.


#include "FishAIPawn.h"

#include "Components/CapsuleComponent.h"


// Sets default values
AFishAIPawn::AFishAIPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating MovementComponent"));
}

// Called when the game starts or when spawned
void AFishAIPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFishAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFishAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UPawnMovementComponent* AFishAIPawn::GetMovementComponent() const
{
	return FloatingPawnMovementComponent;
}

UBehaviorTree* AFishAIPawn::GetBehaviorTree() const
{
	return BehaviorTree;
}

