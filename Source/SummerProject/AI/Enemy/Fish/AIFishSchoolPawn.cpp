// AIFishSchoolPawn.cpp
#include "AIFishSchoolPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "SummerProject/Dev/DEBUG.h"
#include "Math/UnrealMathUtility.h"

AAIFishSchoolPawn::AAIFishSchoolPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create Instanced Static Mesh Component
    InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
    InstancedMeshComponent->bUseDefaultCollision = false;
    RootComponent = InstancedMeshComponent;

    //Init variable
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
        // Init fish school with the mesh and material
        InitializeFish(FishAmount, FishMesh, FishMaterial);  
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Fish Mesh or Material"));
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
    
    //loop for each fish
    for (int32 i = 0; i < NumberOfFish; ++i)
    {
        FTransform InstanceTransform;
        InstanceTransform.SetLocation(UKismetMathLibrary::RandomUnitVector() * FishSpawnRadius);  //spawn fish in random direction in radius
        InstanceTransform.SetRotation(FQuat::Identity);
        InstanceTransform.SetScale3D(FVector(1.0f));
        
        InstancedMeshComponent->AddInstance(InstanceTransform);
        Velocities.Add(FVector::ZeroVector);
        Accelerations.Add(FVector::ZeroVector);
    }
}

void AAIFishSchoolPawn::UpdateFishMovement(float DeltaTime)
{
    FVector SchoolTarget;
    if(TargetActor){SchoolTarget = TargetActor->GetActorLocation();} //location of target actor
    else{SchoolTarget = GetActorLocation();} //if no target actor swim around in self area
    
    for (int32 i = 0; i < InstancedMeshComponent->GetInstanceCount(); ++i)
    {
        FTransform FishTransform;
        InstancedMeshComponent->GetInstanceTransform(i, FishTransform, true);
        FVector FishLocation = FishTransform.GetLocation();
        FVector Vel = Velocities[i];
        
        TArray<FVector> DirectionData = LoopCompute(i, FishTransform);
        
        FVector TargetForce = SteerToward(SchoolTarget - FishLocation, Vel) * TargetWeight;
        FVector CohesionForce = SteerToward(DirectionData[0], Vel)  * CohesionWeight;
        FVector AlignmentForce = SteerToward(DirectionData[1], Vel) * AlignmentWeight;
        FVector SeparationForce = SteerToward(DirectionData[2], Vel) * SeparationWeight;
        
        Accelerations[i] =  TargetForce + CohesionForce + AlignmentForce + SeparationForce; //รวม force ทั้งหมด
        
        FVector AvoidanceDirection = ComputeCollisionAvoidance(i, FishTransform);
        if(!AvoidanceDirection.IsZero())
        {
            Accelerations[i]+= SteerToward(AvoidanceDirection, Vel) * CollisionAvoidanceWeight;
        }
        Accelerations[i] = Accelerations[i].GetClampedToMaxSize(MaxForce);
        
        Velocities[i] += Accelerations[i] * DeltaTime;
        Velocities[i] = Velocities[i].GetClampedToMaxSize(MaxSpeed);
        
        FVector NewLocation = FishLocation + Velocities[i] * DeltaTime;
        FQuat NewRotation = FQuat::Slerp(FishTransform.GetRotation() ,FQuat(Velocities[i].Rotation()),7.75f * DeltaTime);
        FishTransform.SetLocation(NewLocation);
        FishTransform.SetRotation(NewRotation);
        
        InstancedMeshComponent->UpdateInstanceTransform(i, FishTransform, true);

        if(DrawDebug)
        {
            FVector EndLocation = FishLocation + (Velocities[i] * 15.0f);
            DrawDebugLine(
                GetWorld(),       
                FishLocation,      
                EndLocation,
                FColor::Yellow,
                false,
                -1.0f,
                0,
                2.0f
            );
        }
        
    }
    
    InstancedMeshComponent->MarkRenderStateDirty();  // Update rendering
}

TArray<FVector> AAIFishSchoolPawn::LoopCompute(int32 FishIndex, FTransform FishTransform)
{
    //FishIndex = current fish index 
    TArray<FVector> Output;
    Output.SetNum(4);
    Output[0] = Velocities[FishIndex].GetSafeNormal();
    Output[1] = Velocities[FishIndex].GetSafeNormal();
    Output[2] = Velocities[FishIndex].GetSafeNormal();
    
    FVector FishLocation = FishTransform.GetLocation();
    
    int32 NeighborCount = 0;
    //Chohesion
    FVector CenterOfMass = FVector::ZeroVector;
    //Alignment
    FVector AverageDirection = FVector::ZeroVector;
    //Separation
    int32 SeparationNeighborCount = 0;
    FVector SeparationForce = FVector::ZeroVector;

    //count fish in NeighborRadius and SeparationRadius
    for (int32 j = 0; j < InstancedMeshComponent->GetInstanceCount(); ++j) 
    {
        if (j != FishIndex)
        {
            FTransform OtherTransform;
            InstancedMeshComponent->GetInstanceTransform(j, OtherTransform, true);
            FVector OtherLocation = OtherTransform.GetLocation();
            float Distance = FVector::Dist(FishLocation, OtherLocation);
            if (Distance < NeighborRadius)
            {
                CenterOfMass += OtherLocation;
                AverageDirection += Velocities[j];
                NeighborCount++;
            }
            
            if (Distance < SeparationRadius && Distance > 0)
            {
                SeparationForce += (FishLocation - OtherLocation) / Distance;
                SeparationNeighborCount++;
            }
        }
    }
    
    if (NeighborCount > 0) //if there are other fish NeighborRadius compute mass center and average direction
    {
        CenterOfMass /= NeighborCount;
        AverageDirection /= NeighborCount;
        Output[0] = CenterOfMass - FishLocation.GetSafeNormal(); //output direction to center off mass
        Output[1] = AverageDirection.GetSafeNormal(); //output average direction
    }
    else //if no fish in radius output current direction
    {
        Output[0] = Velocities[FishIndex].GetSafeNormal();
        Output[1] = Velocities[FishIndex].GetSafeNormal();
    }
    
    if (SeparationNeighborCount > 0) //if there are other fish in SeperationRadius radius compute mass center and average direction
    {
        SeparationForce /= NeighborCount;

        Output[2] = SeparationForce.GetClampedToMaxSize(MaxForce); //output seperation force
    }
    else //if no fish in radius output 0 vector (no force)
    {
        Output[2] = FVector::ZeroVector;
    }
    
    return Output;
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
        return CenterOfMass - FishLocation.GetSafeNormal();
        //return Seek(CenterOfMass, FishIndex, FishTransform);  // Seek towards the center of mass
    }

    return Velocities[FishIndex].GetSafeNormal();
    //return FVector::ZeroVector;
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
        return AverageVelocity.GetSafeNormal();
        //return (AverageVelocity - Velocities[FishIndex]).GetClampedToMaxSize(MaxForce);
    }

    return Velocities[FishIndex].GetSafeNormal();
    //return FVector::ZeroVector;
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

        return SeparationForce.GetSafeNormal();
        /*// Normalize and scale by MaxSpeed
        SeparationForce = SeparationForce.GetSafeNormal() * MaxSpeed;

        // Compute the steering force by subtracting the current velocity
        SeparationForce -= Velocities[FishIndex];*/
    }

    //return SeparationForce * MaxForce;
    return Velocities[FishIndex].GetSafeNormal();
    //return SeparationForce.GetClampedToMaxSize(MaxForce);
}

FVector AAIFishSchoolPawn::ComputeCollisionAvoidance(int32 FishIndex, FTransform FishTransform)
{
    FVector FishLocation = FishTransform.GetLocation();
    FVector EndLocation = FishLocation + (Velocities[FishIndex].GetSafeNormal() * CollisonAvoidanceTraceDistance);
    FVector AvoidanceForce = FVector::ZeroVector;
    
    // perform a sphere overlap check to detect nearby obstacles
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  // ignore the owning pawn (self)
    QueryParams.AddIgnoredComponent(InstancedMeshComponent); //ignore all mesh instance

    //line trace to check if there is collision in front
    FHitResult CollisionCheckHitResult;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        CollisionCheckHitResult,
        FishLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );

    if(DrawDebug)
    {
        // Draw the debug point at the specified location
        DrawDebugLine(
            GetWorld(),
            FishLocation,
            EndLocation,        
            bHit ? FColor::Red : FColor::Green,
            false,
            0.1f
        );    
    }
    
    
    if(bHit)
    {
        return PerformLineTracesForCollisionAvoidance(FishLocation, CollisonAvoidanceTraceDistance, FishIndex, FishTransform); //cone line trace in front of fish to find right direction
    }
    
    return FVector::ZeroVector; //if not hit don't return force
}

FVector AAIFishSchoolPawn::ComputeCollisionAvoidance2(int32 FishIndex, FTransform FishTransform)
{
    FVector Vnext;
    FVector StartLocation = FishTransform.GetLocation();
    FVector EndLocation = StartLocation + (FishTransform.GetRotation().GetForwardVector() * CollisonAvoidanceTraceDistance);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredComponent(InstancedMeshComponent);
    FHitResult HitResult;

    if (!GetWorld()){return FVector::ZeroVector;}
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,  // Use appropriate collision channel
        QueryParams
    );
    

    if (bHit)
    {
        float w = 5/HitResult.Distance;
        FVector V = FishTransform.GetRotation().GetForwardVector();
        FVector AvoidVector = (HitResult.Location - StartLocation).GetSafeNormal();
        DEBUG::print("V = " + V.ToString());
        DEBUG::print("AvoidVec = " + AvoidVector.ToString());
        float costheta = FVector::DotProduct(V, AvoidVector) / FVector::Distance(V, AvoidVector);
        DEBUG::print("CosTheta = " + FString::SanitizeFloat(costheta));
        if (costheta < PI)
        {
            float theta = w * costheta;
            Vnext.X =  FMath::Cos(theta) * V.X + 0 + FMath::Sin(theta) * V.Z;
            Vnext.Y = V.Y;
            Vnext.Z =  (-1 * FMath::Sin(theta) * V.X) + 0 + FMath::Cos(theta) * V.Z;
        }
        else{ Vnext = FVector::ZeroVector; }
        DEBUG::print("Vnext = " + Vnext.ToString());
        
        FVector e = StartLocation + (Vnext * 100);

        if (GetWorld() && true)
        {
            // Draw the debug point at the specified location
            DrawDebugLine(
                GetWorld(),
                StartLocation,
                e,         // Location of the point
                FColor::Turquoise,            // Color of the point
                false,            // Persistent lines (false means they will disappear after 'Duration')
                0.1,
                0,
                bHit ? 0 : 0.3
            );
        }
    }
    
    return Vnext;
    
}


// Method to initialize view directions using the Fibonacci Sphere method
//get directions of the line to line trace in cone shape
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
FVector AAIFishSchoolPawn::PerformLineTracesForCollisionAvoidance(const FVector& StartLocation, float TraceDistance, int32 FishIndex, FTransform FishTransform)
{
    // Ensure directions are initialized
    
    if (ViewDirections.Num() == 0)
    {
        InitializeViewDirections(); //get directions of the line to line trace in cone shape
    }
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  // Ignore the current actor (self)
    QueryParams.AddIgnoredComponent(InstancedMeshComponent);
    
    for (const FVector& Direction : ViewDirections) //for each direction do line trace
    {
        FVector GlobalDirection = FishTransform.TransformVector(Direction);
        FVector AvoidDirection = GlobalDirection;
        FVector EndLocation = StartLocation + (GlobalDirection * TraceDistance);
        
        FHitResult HitResult;
        // perform line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            ECC_Visibility,
            QueryParams
        );
        
        
        // draw debug
        if (GetWorld() && DrawDebug)
        {
            DrawDebugLine(
                GetWorld(),
                StartLocation,
                EndLocation,
                bHit ? FColor::Blue : FColor::Cyan,
                false,
                0.1,
                0,
                bHit ? 0 : 0.3
            );
        }

        if (!bHit) //if the line doesn't hit return that direction as direction to avoid collision
        {
            return AvoidDirection;
        }
    }
    return Velocities[FishIndex].GetSafeNormal();
}

FVector AAIFishSchoolPawn::SteerToward(FVector vector, FVector Velocity)
{
    if(vector.IsZero()){return FVector::ZeroVector;}
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

