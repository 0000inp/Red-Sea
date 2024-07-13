
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"

#include "Submarine.generated.h"

class UArrowComponent;
class APlayerCharacter;
class UInteractionComponent;

USTRUCT(BlueprintType)
struct FSubmarineMovementSpeed
{
	GENERATED_BODY()
	float Forward;
	float Backward;
	float Up;
	float Down;
};

USTRUCT(BlueprintType)
struct FMovementValue
{
	GENERATED_BODY()
	float Horizontal;
	float Vertical;
};

UCLASS()
class SUMMERPROJECT_API ASubmarine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASubmarine();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UArrowComponent> ArrowComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MoveForce = 50000.0f;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FSubmarineMovementSpeed MoveSpeed = {1, 1, 1, 1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FMovementValue MoveValue = {0, 0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float TurnRate = 1.5f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveUp();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveDown();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveForward(float DeltaTime);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveBackward();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveHorizontal(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveVertical(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TurnLeft();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TurnRight();

	UFUNCTION(BlueprintCallable)
	void RotateTowardsArrow(float DeltaTime);
};
