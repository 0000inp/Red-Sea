// Fill out your copyright notice in the Description page of Project Settings.

#include "Submarine.h"

#include <string>

#include "Components/ArrowComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SummerProject/Dev/DEBUG.h"

// Sets default values
ASubmarine::ASubmarine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubmarineMesh"));
	
	RootComponent = Mesh;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(Mesh);
	
	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(Mesh);
	
	Mesh->SetSimulatePhysics(true);
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
	
	Speed = FMath::Lerp(Speed, ControlSpeed, DeltaTime * Acceleration);
	Move(DeltaTime);
}

void ASubmarine::MoveUp_Implementation()
{
	Mesh->AddForce({0, 0, MoveForce * MoveSpeed.Up}, NAME_None, true);
}

void ASubmarine::MoveDown_Implementation()
{
	Mesh->AddForce({0,0,MoveForce * MoveSpeed.Down * -1}, NAME_None, true);
}

void ASubmarine::Move_Implementation(float DeltaTime)
{
	Mesh->AddForce(Mesh->GetForwardVector() * MoveForce * Speed, NAME_None, true);
}


void ASubmarine::MoveHorizontal_Implementation(float DeltaTime)
{
	RotateTowardsArrow(DeltaTime);
	Mesh->AddForce(Mesh->GetUpVector() * MoveForce * MoveValue.Horizontal, NAME_None, true);
}

void ASubmarine::MoveVertical_Implementation(float DeltaTime)
{
	Mesh->AddForce(Mesh->GetForwardVector() * MoveForce * MoveValue.Vertical, NAME_None, true);
}

void ASubmarine::TurnLeft_Implementation()
{
	
}


void ASubmarine::TurnRight_Implementation()
{
	
}

