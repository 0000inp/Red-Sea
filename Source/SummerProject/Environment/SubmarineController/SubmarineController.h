#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"
#include "SummerProject/Interface/SubmarineControl.h"

#include "SubmarineController.generated.h"

class ASubmarine;

UCLASS()
class SUMMERPROJECT_API ASubmarineController : public AActor, public ISubmarineControl
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASubmarineController();
	
	UPROPERTY(EditAnywhere);
	TObjectPtr<USceneComponent> Root = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actor Component|Interaction");
	TObjectPtr<UInteractionComponent> InteractionComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void ControllerInteracted(APlayerCharacter* Player);

	bool isBeingUse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	ASubmarine* Submarine = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
	virtual void ControlMovement(FVector2d MoveVector) override;
	virtual void ControlRotation(FVector2d LookVector) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RotateSubmarine(FRotator Rotator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveSubmarine(float speed);

	virtual void BeingUse() override;
	virtual void StopBeingUse() override;
};