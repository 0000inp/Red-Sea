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

/*void ASubmarine::RotateTowardsArrow(float DeltaTime)
{
	FQuat CapsuleQuat = CapsuleComponent->GetComponentQuat();
	
	//FQuat CapsuleQuat = CapsuleComponent->GetUpVector().ToOrientationQuat();
	FQuat ArrowQuat = ArrowComponent->GetComponentQuat();
	
	FVector dir = ArrowComponent->GetForwardVector();
	FQuat LookAtQuat = FQuat::FindBetweenVectors(CapsuleComponent->GetForwardVector(), dir);
	
	
	float AngularDistance = CapsuleQuat.AngularDistance(ArrowQuat);
	//DEBUG::print(FString::SanitizeFloat(AngularDistance));
	if(AngularDistance > 0)
	{
		FQuat LerpQuat = FQuat::Slerp(CapsuleQuat, ArrowQuat, DeltaTime * TurnRate);
		SetActorRotation(LerpQuat);
	}
	
}*/

void ASubmarine::RotateTowardsArrow(float DeltaTime)
{
	// Get the current forward vector of the submarine (Capsule)
	FVector CurrentForward = Mesh->GetForwardVector();

	// Get the target direction from the arrow
	FVector TargetForward = ArrowComponent->GetForwardVector();

	// Zero out the Z-axis (vertical axis) to only affect the yaw (horizontal rotation)
	CurrentForward.Z = 0.0f;
	TargetForward.Z = 0.0f;

	// Normalize the vectors to ensure proper calculations
	CurrentForward.Normalize();
	TargetForward.Normalize();

	// Calculate the difference in yaw between the current and target directions
	FQuat CurrentQuat = FQuat::FindBetweenVectors(FVector::ForwardVector, CurrentForward);
	FQuat TargetQuat = FQuat::FindBetweenVectors(FVector::ForwardVector, TargetForward);

	// Calculate the angular distance between the two quaternions
	float AngularDistance = CurrentQuat.AngularDistance(TargetQuat);

	if (AngularDistance > 0.1) // Check if there's a significant difference
	{
		// Slerp between the current and target rotation based on DeltaTime and TurnRate
		FQuat LerpQuat = FQuat::Slerp(CurrentQuat, TargetQuat, DeltaTime * TurnRate);

		// Convert the resulting quaternion to a rotator and only apply the yaw (horizontal rotation)
		FRotator LerpRotator = LerpQuat.Rotator();

		// Get the current rotation and set only the yaw
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = LerpRotator.Yaw;

		// Apply the new yaw rotation to the submarine
		SetActorRotation(NewRotation);
	}
}

