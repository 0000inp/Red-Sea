// Fill out your copyright notice in the Description page of Project Settings.

#include "Submarine.h"

#include "Components/ArrowComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SummerProject/Dev/DEBUG.h"

// Sets default values
ASubmarine::ASubmarine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	Mesh->SetupAttachment(CapsuleComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(CapsuleComponent);
	
	CapsuleComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ASubmarine::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASubmarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASubmarine::MoveUp_Implementation()
{
	CapsuleComponent->AddForce({0, 0, MoveForce * MoveSpeed.Up});
}

void ASubmarine::MoveDown_Implementation()
{
	CapsuleComponent->AddForce({0,0,MoveForce * MoveSpeed.Down * -1});
}

void ASubmarine::MoveForward_Implementation(float DeltaTime)
{
	RotateTowardsArrow(DeltaTime);
	CapsuleComponent->AddForce(CapsuleComponent->GetUpVector() * MoveForce * MoveSpeed.Forward);
}

void ASubmarine::MoveBackward_Implementation()
{
	CapsuleComponent->AddForce(CapsuleComponent->GetUpVector() * MoveForce * MoveSpeed.Backward * -1);
}

void ASubmarine::MoveHorizontal_Implementation(float DeltaTime)
{
	RotateTowardsArrow(DeltaTime);
	CapsuleComponent->AddForce(CapsuleComponent->GetUpVector() * MoveForce * MoveValue.Horizontal);
}

void ASubmarine::MoveVertical_Implementation(float DeltaTime)
{
	CapsuleComponent->AddForce(CapsuleComponent->GetUpVector() * MoveForce * MoveValue.Vertical);
}

void ASubmarine::TurnLeft_Implementation()
{
	
}


void ASubmarine::TurnRight_Implementation()
{
	
}

void ASubmarine::RotateTowardsArrow(float DeltaTime)
{
	FQuat CapsuleQuat = CapsuleComponent->GetComponentQuat();
	
	//FQuat CapsuleQuat = CapsuleComponent->GetUpVector().ToOrientationQuat();
	FQuat ArrowQuat = ArrowComponent->GetComponentQuat();
	
	FVector dir = ArrowComponent->GetForwardVector();
	FQuat LookAtQuat = FQuat::FindBetweenVectors(CapsuleComponent->GetForwardVector(), dir);
	
	
	float AngularDistance = CapsuleQuat.AngularDistance(ArrowQuat);
	DEBUG::print(FString::SanitizeFloat(AngularDistance));
	if(AngularDistance > 0)
	{
		FQuat LerpQuat = FQuat::Slerp(CapsuleQuat, ArrowQuat, DeltaTime * TurnRate);
		SetActorRotation(LerpQuat);
	}
	
}

