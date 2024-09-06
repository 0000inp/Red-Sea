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
        
        /*FVector TargetForce = Seek(SchoolTarget, i, FishTransform) * TargetWeight;
        FVector CohesionForce = ComputeCohesion(i, FishTransform) * CohesionWeight;
        FVector AlignmentForce = ComputeAlignment(i, FishTransform) * AlignmentWeight;
        FVector SeparationForce = ComputeSeparation(i, FishTransform) * SeparationWeight;
        FVector CollisionAvoidanceForce = ComputeCollisionAvoidance(i, FishTransform) * CollisionAvoidanceWeight;*/

        FVector Vel = Velocities[i];
        
        FVector TargetForce = SteerToward(SchoolTarget - FishLocation, Vel) * TargetWeight;
        FVector CohesionForce = SteerToward(ComputeCohesion(i, FishTransform), Vel)  * CohesionWeight;
        FVector AlignmentForce = SteerToward(ComputeAlignment(i, FishTransform), Vel) * AlignmentWeight;
        FVector SeparationForce = SteerToward(ComputeSeparation(i, FishTransform), Vel) * SeparationWeight;
        FVector CollisionAvoidanceForce = ComputeCollisionAvoidance(i, FishTransform) * CollisionAvoidanceWeight;
        
        Accelerations[i] =  TargetForce + CohesionForce + AlignmentForce + SeparationForce + CollisionAvoidanceForce; //รวม force ทั้งหมด
        Accelerations[i] = Accelerations[i].GetClampedToMaxSize(MaxForce);

        DEBUG::print("target : " + FString::SanitizeFloat(TargetForce.Size()));
        DEBUG::print("cohesion : " + FString::SanitizeFloat(CohesionForce.Size()));
        DEBUG::print("align : " + FString::SanitizeFloat(AlignmentForce.Size()));
        DEBUG::print("separate : " + FString::SanitizeFloat(SeparationForce.Size()));
        DEBUG::print("colli : " + FString::SanitizeFloat(CollisionAvoidanceForce.Size()));
        
        Velocities[i] += Accelerations[i] * DeltaTime;
        float speed = Velocities[i].Size();
        FVector dir = Velocities[i] / speed;
        speed =  UKismetMathLibrary::Clamp(speed, 0.0f, MaxSpeed);
        Velocities[i] = dir * speed;
        //Velocities[i] = Velocities[i].GetClampedToMaxSize(MaxSpeed);
        
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
            FColor::Yellow,              // Line color
            false,                    // Persistent lines (set to false for one frame, true for persistent lines)
            -1.0f,                    // Lifetime of the line (-1 for persistent if 'bPersistentLines' is true)
            0,                        // Depth priority (0 is fine)
            2.0f                      // Line thickness
        );
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
    FVector FishLocation = FishTransform.GetLocation();
    
    FVector AvoidanceForce = FVector::ZeroVector;
    
    // Perform a sphere overlap check to detect nearby obstacles
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  // Ignore the owning pawn (self)
    QueryParams.AddIgnoredComponent(InstancedMeshComponent); //ignore all mesh instance
    
    FHitResult CollisionCheckHitResult;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        CollisionCheckHitResult,
        FishLocation,
        FishLocation + (FishTransform.GetRotation().GetForwardVector() * CollisonAvoidanceTraceDistance),
        ECC_Visibility,  // Use appropriate collision channel
        QueryParams
    );

    if(bHit)
    {
        FVector AvoidDirection = PerformLineTracesForCollisionAvoidance(FishLocation, CollisonAvoidanceTraceDistance, FishTransform); 
        AvoidanceForce = SteerToward(AvoidDirection, Velocities[FishIndex]);
    }
    
    // Draw the debug point at the specified location
    DrawDebugLine(
        GetWorld(),
        FishLocation,
        FishLocation + (FishTransform.GetRotation().GetForwardVector() * CollisonAvoidanceTraceDistance),        
        bHit ? FColor::Red : FColor::Green,            // Color of the point
        false,            // Persistent lines (false means they will disappear after 'Duration')
        0.1f
    );
    
    // Clamp the avoidance force to avoid erratic behavior
    return AvoidanceForce;
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

        ViewDirections[i] = FVector(x, y, z); ViewDirections[i];
        ViewDirections[i] = ViewDirections[i].RotateAngleAxis(90, FVector(1,0,0)).RotateAngleAxis(90, FVector(0,0,1));
    }
}

// Method to perform line traces using the generated directions
FVector AAIFishSchoolPawn::PerformLineTracesForCollisionAvoidance(const FVector& StartLocation, float TraceDistance, FTransform FishTransform)
{
    // Ensure directions are initialized
    
    if (ViewDirections.Num() == 0)
    {
        InitializeViewDirections();
    }
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  // Ignore the current actor (self)
    QueryParams.AddIgnoredComponent(InstancedMeshComponent);
    
    for (const FVector& Direction : ViewDirections)
    {
        FVector GlobalDirection = FishTransform.TransformVector(Direction );
        FVector AvoidDirection = GlobalDirection;
        FVector EndLocation = StartLocation + (GlobalDirection * TraceDistance);
        
        FHitResult HitResult;
        // Perform the line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            ECC_Visibility,  // Use appropriate collision channel
            QueryParams
        );
        

        // Draw debug line for visualization
        if (GetWorld())
        {
            // Draw the debug point at the specified location
            DrawDebugLine(
                GetWorld(),
                StartLocation,
                EndLocation,         // Location of the point
                bHit ? FColor::Blue : FColor::Cyan,            // Color of the point
                false,            // Persistent lines (false means they will disappear after 'Duration')
                0.1,
                0,
                bHit ? 0 : 0.3
            );
        }

        if (!bHit)
        {
            return AvoidDirection;
        }
    }
    return FVector::ZeroVector;
}

FVector AAIFishSchoolPawn::SteerToward(FVector vector, FVector Velocity)
{
    FVector v = ( vector.GetSafeNormal() * MaxSpeed ) - Velocity;
    return v.GetClampedToMaxSize(MaxForce);
}

FVector AAIFishSchoolPawn::Seek(const FVector& TargetLocation, int32 FishIndex, FTransform FishTransform)
{
    FVector FishLocation = FishTransform.GetLocation();
    
    FVector Desired = (TargetLocation - FishLocation).GetSafeNormal() * MaxSpeed;
    FVector Steering = (Desired - Velocities[FishIndex]).GetClampedToMaxSize(MaxForce);
    return Steering;
}

