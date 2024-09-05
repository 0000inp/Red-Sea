// AIFishSchoolPawn.cpp
#include "AIFishSchoolPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "SummerProject/Dev/DEBUG.h"

AAIFishSchoolPawn::AAIFishSchoolPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create Instanced Static Mesh Component
    InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
    InstancedMeshComponent->bUseDefaultCollision = false;
    RootComponent = InstancedMeshComponent;
    
    FishAmount = 10;
    
    MaxSpeed = 300.0f;
    MaxForce = 10.0f;
    
    TargetWeight = 1.0f;
    CohesionWeight = 1.0f;
    AlignmentWeight = 1.0f;
    SeparationWeight = 1.5f;
    CollisionAvoidanceWeight = 1.0f;
    
    NeighborRadius = 300.0f;
    SeparationRadius = 100.0f;
    CollisionAvoidanceRadius = 500.0f;
}

void AAIFishSchoolPawn::BeginPlay()
{
    Super::BeginPlay();
    
    
    if (FishMesh && FishMaterial)
    {
        // Initialize the fish school with the mesh and material
        InitializeFish(FishAmount, FishMesh, FishMaterial);  // Initialize with 100 fish
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Fish Mesh or Material!"));
        
    }
}

void AAIFishSchoolPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update fish movement
    UpdateFishMovement(DeltaTime);
}

void AAIFishSchoolPawn::InitializeFish(int32 NumberOfFish, UStaticMesh* Mesh, UMaterialInterface* Material)
{
    InstancedMeshComponent->SetStaticMesh(Mesh);
    InstancedMeshComponent->SetMaterial(0, Material);
    
    for (int32 i = 0; i < NumberOfFish; ++i)
    {
        FTransform InstanceTransform;
        InstanceTransform.SetLocation(UKismetMathLibrary::RandomUnitVector() * 500.0f);
        InstanceTransform.SetRotation(FQuat::Identity);
        InstanceTransform.SetScale3D(FVector(1.0f));
        
        InstancedMeshComponent->AddInstance(InstanceTransform);
        Velocities.Add(UKismetMathLibrary::RandomUnitVector());
        Accelerations.Add(FVector::ZeroVector);
    }
}

void AAIFishSchoolPawn::UpdateFishMovement(float DeltaTime)
{
    FVector SchoolTarget;
    if(TargetActor){SchoolTarget = TargetActor->GetActorLocation();}
    else{SchoolTarget = GetActorLocation();  /*The fish should follow this pawn's location*/}
    
    for (int32 i = 0; i < InstancedMeshComponent->GetInstanceCount(); ++i)
    {
        FTransform FishTransform;
        InstancedMeshComponent->GetInstanceTransform(i, FishTransform, true);
        FVector FishLocation = FishTransform.GetLocation();
        
        FVector TargetForce = Seek(SchoolTarget, i, FishTransform) * TargetWeight;
        FVector CohesionForce = ComputeCohesion(i, FishTransform) * CohesionWeight;
        FVector AlignmentForce = ComputeAlignment(i, FishTransform) * AlignmentWeight;
        FVector SeparationForce = ComputeSeparation(i, FishTransform) * SeparationWeight;
        FVector CollisionAvoidanceForce = ComputeCollisionAvoidance(i, FishTransform) * CollisionAvoidanceWeight;
        
        
        Accelerations[i] =  TargetForce + CohesionForce + AlignmentForce + SeparationForce + CollisionAvoidanceForce; //รวม force ทั้งหมด
        Accelerations[i] = Accelerations[i].GetClampedToMaxSize(MaxForce);
        
        //DEBUG::print(CollisionAvoidanceForce.ToString());
        
        Velocities[i] += Accelerations[i] * DeltaTime;
        Velocities[i] = Velocities[i].GetClampedToMaxSize(MaxSpeed);
        
        FVector NewLocation = FishLocation + Velocities[i] * DeltaTime;
        FQuat NewRotation = FQuat::Slerp(FishTransform.GetRotation() ,FQuat(Velocities[i].Rotation()),7.75f * DeltaTime);
        FishTransform.SetLocation(NewLocation);
        FishTransform.SetRotation(NewRotation);
        
        InstancedMeshComponent->UpdateInstanceTransform(i, FishTransform, true);

        FVector EndLocation = FishLocation + (Velocities[i] * 15.0f);  // Scale the velocity for visualization purposes
        DrawDebugLine(
            GetWorld(),               // The world context
            FishLocation,          // Start location
            EndLocation,              // End location
            FColor::Red,              // Line color
            false,                    // Persistent lines (set to false for one frame, true for persistent lines)
            -1.0f,                    // Lifetime of the line (-1 for persistent if 'bPersistentLines' is true)
            0,                        // Depth priority (0 is fine)
            2.0f                      // Line thickness
        );
        PerformLineTracesForCollisionAvoidance(FishLocation, SphereTraceDistance);
    }
    
    InstancedMeshComponent->MarkRenderStateDirty();  // Update rendering
}

FVector AAIFishSchoolPawn::ComputeCohesion(int32 FishIndex, FTransform FishTransform)
{
    FVector CenterOfMass = FVector::ZeroVector;
    int32 NeighborCount = 0;
    
    FVector FishLocation = FishTransform.GetLocation();
    
    for (int32 j = 0; j < InstancedMeshComponent->GetInstanceCount(); ++j)
    {
        if (j != FishIndex)
        {
            FTransform OtherTransform;
            InstancedMeshComponent->GetInstanceTransform(j, OtherTransform, true);
            FVector OtherLocation = OtherTransform.GetLocation();
            
            if (FVector::Dist(FishLocation, OtherLocation) < NeighborRadius)
            {
                CenterOfMass += OtherLocation;
                NeighborCount++;
            }
        }
    }

    if (NeighborCount > 0)
    {
        CenterOfMass /= NeighborCount;
        return Seek(CenterOfMass, FishIndex, FishTransform);  // Seek towards the center of mass
    }

    return FVector::ZeroVector;
}

FVector AAIFishSchoolPawn::ComputeAlignment(int32 FishIndex, FTransform FishTransform)
{
    FVector AverageVelocity = FVector::ZeroVector;
    int32 NeighborCount = 0;

    for (int32 j = 0; j < InstancedMeshComponent->GetInstanceCount(); ++j)
    {
        if (j != FishIndex)
        {
            FTransform OtherTransform;
            InstancedMeshComponent->GetInstanceTransform(j, OtherTransform, true);
            FVector OtherLocation = OtherTransform.GetLocation();
            
            FVector FishLocation = FishTransform.GetLocation();

            if (FVector::Dist(FishLocation, OtherLocation) < NeighborRadius)
            {
                AverageVelocity += Velocities[j];
                NeighborCount++;
            }
        }
    }

    if (NeighborCount > 0)
    {
        AverageVelocity /= NeighborCount;
        return (AverageVelocity - Velocities[FishIndex]).GetClampedToMaxSize(MaxForce);
    }

    return FVector::ZeroVector;
}

FVector AAIFishSchoolPawn::ComputeSeparation(int32 FishIndex, FTransform FishTransform)
{
    FVector SeparationForce = FVector::ZeroVector;
    int32 NeighborCount = 0;

    for (int32 j = 0; j < InstancedMeshComponent->GetInstanceCount(); ++j)
    {
        if (j != FishIndex)
        {
            FTransform OtherTransform;
            InstancedMeshComponent->GetInstanceTransform(j, OtherTransform, true);
            FVector OtherLocation = OtherTransform.GetLocation();
            
            FVector FishLocation = FishTransform.GetLocation();
            
            float Distance = FVector::Dist(FishLocation, OtherLocation);
            if (Distance < SeparationRadius && Distance > 0)
            {
                SeparationForce += (FishLocation - OtherLocation).GetSafeNormal() / Distance;
                NeighborCount++;
            }
        }
    }

    if (NeighborCount > 0)
    {
        // Average the separation force
        SeparationForce /= NeighborCount;
        
        // Normalize and scale by MaxSpeed
        SeparationForce = SeparationForce.GetSafeNormal() * MaxSpeed;

        // Compute the steering force by subtracting the current velocity
        SeparationForce -= Velocities[FishIndex];
    }

    //return SeparationForce * MaxForce;
    return SeparationForce.GetClampedToMaxSize(MaxForce);
}

FVector AAIFishSchoolPawn::ComputeCollisionAvoidance(int32 FishIndex, FTransform FishTransform)
{
    FVector CurrentLocation = FishTransform.GetLocation();
    
    FVector AvoidanceForce = FVector::ZeroVector;
    
    // Define the radius for collision avoidance
    float AvoidanceRadius = CollisionAvoidanceRadius;  // Adjust as needed for the size of the fish and obstacles
    
    // Perform a sphere overlap check to detect nearby obstacles
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  // Ignore the owning pawn (self)
    QueryParams.AddIgnoredComponent(InstancedMeshComponent); //ignore all mesh instance
    
    // Check for nearby obstacles using a sphere trace
    bool bHasHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        CurrentLocation,
        FQuat::Identity,
        ECC_WorldStatic,  // Use appropriate collision channel
        FCollisionShape::MakeSphere(AvoidanceRadius),
        QueryParams
    );

    if (bHasHit)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                // Compute the direction away from the obstacle
                FVector ToObstacle = CurrentLocation - HitActor->GetActorLocation();
                float Distance = ToObstacle.Size();
                if (Distance > 0)
                {
                    FVector CurrentVelocity = Velocities[FishIndex];
                    FVector w = FVector(5/Distance * CollisionAvoidanceWeight,5/Distance * CollisionAvoidanceWeight,5/Distance * CollisionAvoidanceWeight);
                    FVector x = (CurrentVelocity * ToObstacle) / (CurrentVelocity.Size() * Distance);
                    float theta = FVector::DotProduct(x, w);

                    if (FMath::Abs(theta) < PI / 4)
                    {
                        float CosTheta = FMath::Cos(theta);
                        float SinTheta = FMath::Sin(theta);

                        // Rotate the velocity vector according to the formula
                        FVector RotatedVelocity;
                        RotatedVelocity.X = CosTheta * CurrentVelocity.X + SinTheta * CurrentVelocity.Z;
                        RotatedVelocity.Y = CurrentVelocity.Y;
                        RotatedVelocity.Z = -SinTheta * CurrentVelocity.X + CosTheta * CurrentVelocity.Z;

                        // Update the velocity for the Boid instance
                        AvoidanceForce += RotatedVelocity;

                        FVector EndLocation = FishTransform.GetLocation() + (AvoidanceForce * 20.0f);  // Scale the velocity for visualization purposes
                        DrawDebugLine(
                            GetWorld(),               // The world context
                            FishTransform.GetLocation(),          // Start location
                            EndLocation,              // End location
                            FColor::Green,              // Line color
                            false,                    // Persistent lines (set to false for one frame, true for persistent lines)
                            -1.0f,                    // Lifetime of the line (-1 for persistent if 'bPersistentLines' is true)
                            0,                        // Depth priority (0 is fine)
                            2.0f                      // Line thickness
                        );
                    }
                    
                    // Calculate avoidance force inversely proportional to the distance
                    //AvoidanceForce += ToObstacle.GetSafeNormal() / Distance;
                    
                }
            }
        }
    }

    // Clamp the avoidance force to avoid erratic behavior
    return AvoidanceForce.GetClampedToMaxSize(MaxForce);
}

FVector AAIFishSchoolPawn::Seek(const FVector& TargetLocation, int32 FishIndex, FTransform FishTransform)
{
    FVector FishLocation = FishTransform.GetLocation();
    
    FVector Desired = (TargetLocation - FishLocation).GetSafeNormal() * MaxSpeed;
    FVector Steering = (Desired - Velocities[FishIndex]).GetClampedToMaxSize(MaxForce);
    return Steering;
}

// Method to initialize view directions using the Fibonacci Sphere method
void AAIFishSchoolPawn::InitializeViewDirections()
{
    ViewDirections.SetNum(NumViewDirections);

    float GoldenRatio = (1 + FMath::Sqrt(5.0f)) / 2.0f;  // Golden ratio
    float AngleIncrement = PI * 2.0f * GoldenRatio;       // Angle increment

    for (int32 i = 0; i < NumViewDirections; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(NumViewDirections);
        float Inclination = FMath::Acos(1 - 2 * t);  // Vertical angle
        float Azimuth = AngleIncrement * i;          // Horizontal angle

        // Convert spherical coordinates to Cartesian coordinates
        float x = FMath::Sin(Inclination) * FMath::Cos(Azimuth);
        float y = FMath::Sin(Inclination) * FMath::Sin(Azimuth);
        float z = FMath::Cos(Inclination);

        ViewDirections[i] = FVector(x, y, z);
    }
}

// Method to perform line traces using the generated directions
void AAIFishSchoolPawn::PerformLineTracesForCollisionAvoidance(const FVector& StartLocation, float TraceDistance)
{
    // Ensure directions are initialized
    if (ViewDirections.Num() == 0)
    {
        InitializeViewDirections();
    }

    for (const FVector& Direction : ViewDirections)
    {
        FVector EndLocation = StartLocation + (Direction * TraceDistance);

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);  // Ignore the current actor (self)
        QueryParams.AddIgnoredComponent(InstancedMeshComponent);
        // Perform the line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            ECC_Visibility,  // Use appropriate collision channel
            QueryParams
        );

        // Draw debug line for visualization
        FColor LineColor = bHit ? FColor::Red : FColor::Green;  // Red if hit, green if no hit
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 0.5f, 0, 1.0f);
        
        if (GetWorld())
        {
            // Draw the debug point at the specified location
            DrawDebugPoint(
                GetWorld(),
                EndLocation,         // Location of the point
                1.0f,        // Size of the point
                FColor::Blue,            // Color of the point
                false,            // Persistent lines (false means they will disappear after 'Duration')
                1          // Time (seconds) the point will be visible
            );
        }

        // If a collision is detected, adjust fish movement
        if (bHit)
        {
            /*//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.Actor->GetName());

            // Collision avoidance logic: adjust the fish's direction away from the obstacle
            FVector AvoidanceForce = StartLocation - HitResult.ImpactPoint;
            AvoidanceForce.Normalize();
            AvoidanceForce *= 100.0f;  // Adjust this multiplier to control the avoidance strength
            // Apply the avoidance force to the boid's movement direction (pseudo code)
            // BoidVelocity += AvoidanceForce;*/
        }
    }
}

