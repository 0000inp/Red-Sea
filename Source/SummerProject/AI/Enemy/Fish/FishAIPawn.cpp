// Fill out your copyright notice in the Description page of Project Settings.


#include "FishAIPawn.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SummerProject/ActorComponents/HealthComponent/HealthComponent.h"
#include "SummerProject/Dev/DEBUG.h"


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

void AFishAIPawn::Attack()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation;
	//TraceStart + (Camera->GetForwardVector() * TraceDistance);
	
	float Radius = AttackRadius;
	TArray<FHitResult> HitResults;

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(Radius);
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionSphere
	);
	
	DrawDebugSphere(
		GetWorld(),
		StartLocation,
		Radius,
		12,                      
		FColor::Green,
		false,                    
		2.0f,                     
		0,                       
		2.0f                      
	);

	if (bHit)
	{
		for (auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			DEBUG::print(HitActor->GetName());
			if (HitActor && HitActor != this)
			{
				if (UHealthComponent* HealthComponent = HitActor->FindComponentByClass<UHealthComponent>())
				{
					HealthComponent->TakeDamage(AttackDamage);
				}
			}
		}
	}
	
	// Play attack animation, sound, etc.
	
}


