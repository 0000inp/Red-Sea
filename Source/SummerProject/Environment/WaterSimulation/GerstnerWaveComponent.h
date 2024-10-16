// GerstnerWaveComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GerstnerWaveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERPROJECT_API UGerstnerWaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGerstnerWaveComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Properties")
	float Amplitude = 100.0f;  // Height of wave

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Properties")
	float Wavelength = 500.0f; // Distance between wave peaks

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Properties")
	float Speed = 20.0f; // Speed of wave

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Properties")
	FVector2D Direction = FVector2D(1.0f, 0.0f); // Direction of wave (normalized)

	// Time at which the wave starts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Properties")
	float StartTime = 0.0f;

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function to calculate Gerstner Wave displacement
	FVector CalculateGerstnerWave(float Time, FVector WorldPosition);

	// Update mesh vertex positions
	void UpdateMesh();
};
