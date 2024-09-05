// AIFishSchoolPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "AIFishSchoolPawn.generated.h"

UCLASS()
class SUMMERPROJECT_API AAIFishSchoolPawn : public APawn
{
	GENERATED_BODY()

public:
	AAIFishSchoolPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Instanced Static Mesh Component to manage multiple fis
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fish School")
	UInstancedStaticMeshComponent* InstancedMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* FishMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UMaterialInterface* FishMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	// Boid behavior parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float FishAmount;
	
	// Movement parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float MaxForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float TargetWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float CohesionWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float AlignmentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float SeparationWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float CollisionAvoidanceWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float NeighborRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float SeparationRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior")
	float CollisionAvoidanceRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior|Golden Sphere")
	int32 SphereTraceDistance = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Behavior|Golden Sphere")
	int32 NumViewDirections = 200;
	
	TArray<FVector> ViewDirections;

	void InitializeFish(int32 NumberOfFish, UStaticMesh* FishMesh, UMaterialInterface* FishMaterial);

private:
	TArray<FVector> Velocities;
	TArray<FVector> Accelerations;
	
	// Update all fish movement
	void UpdateFishMovement(float DeltaTime);
	
	// Boid behaviors
	FVector ComputeCohesion(int32 FishIndex, FTransform FishTransform);
	FVector ComputeAlignment(int32 FishIndex, FTransform FishTransform);
	FVector ComputeSeparation(int32 FishIndex, FTransform FishTransform);
	FVector ComputeCollisionAvoidance(int32 FishIndex, FTransform FishTransform);
	FVector Seek(const FVector& TargetLocation, int32 FishIndex, FTransform FishTransform);


	// Define constants for the Fibonacci sphere generation
	
	
	void InitializeViewDirections();
	void PerformLineTracesForCollisionAvoidance(const FVector& StartLocation, float TraceDistance);
};
