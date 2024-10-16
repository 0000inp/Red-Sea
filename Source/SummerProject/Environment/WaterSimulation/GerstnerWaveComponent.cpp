// GerstnerWaveComponent.cpp
#include "GerstnerWaveComponent.h"

#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UGerstnerWaveComponent::UGerstnerWaveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGerstnerWaveComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Main function to calculate wave displacement for Gerstner Wave
FVector UGerstnerWaveComponent::CalculateGerstnerWave(float Time, FVector WorldPosition)
{
	// Normalize the direction
	FVector2D NormDir = Direction.GetSafeNormal();
	FVector2D Pos2D(WorldPosition.X, WorldPosition.Y);

	// Dot product of position and direction to get the wave factor
	float WaveFactor = FVector2D::DotProduct(NormDir, Pos2D);

	// Calculate wave displacement
	float Theta = (WaveFactor / Wavelength) + (Speed * Time);
	float WaveHeight = Amplitude * FMath::Sin(Theta);

	// Gerstner displacement in X and Y direction
	FVector Displacement = FVector(
		Amplitude * NormDir.X * FMath::Cos(Theta),
		Amplitude * NormDir.Y * FMath::Cos(Theta),
		WaveHeight
	);

	return Displacement;
}

void UGerstnerWaveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update mesh or water surface based on wave simulation
	UpdateMesh();
}

// Function to update mesh or vertex positions
void UGerstnerWaveComponent::UpdateMesh()
{
	// Assuming you have a reference to a procedural mesh component
	UProceduralMeshComponent* Mesh = GetOwner()->FindComponentByClass<UProceduralMeshComponent>();
	if (!Mesh) return;
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	// Get current vertices from the procedural mesh
	//Mesh->GetProcMeshSection(0, Vertices, Triangles, Normals, UVs, Tangents, VertexColors);
	FProcMeshSection* ProcMeshSection = Mesh->GetProcMeshSection(0);
	// Now manually retrieve data from the section
	for (const FProcMeshVertex& Vertex : ProcMeshSection->ProcVertexBuffer)
	{
		Vertices.Add(Vertex.Position);
		Normals.Add(Vertex.Normal);
		UVs.Add(Vertex.UV0);
		Tangents.Add(Vertex.Tangent);
		VertexColors.Add(Vertex.Color);
	}
	
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Modify each vertex based on the Gerstner wave
	for (FVector& Vertex : Vertices)
	{
		// Original vertex position before applying waves
		FVector OriginalPosition = Vertex;

		// Calculate the displacement using Gerstner wave formula
		FVector Displacement = CalculateGerstnerWave(CurrentTime, OriginalPosition);

		// Apply displacement to vertex
		Vertex = OriginalPosition + Displacement;
	}

	// Update the procedural mesh section with new vertices
	Mesh->UpdateMeshSection(0, Vertices, Normals, UVs, VertexColors, Tangents);
}
