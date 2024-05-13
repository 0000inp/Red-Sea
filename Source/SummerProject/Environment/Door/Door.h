// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UInteractionComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SUMMERPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();
	
	UPROPERTY(EditAnywhere);
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<UStaticMeshComponent> DoorPanel = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<UStaticMeshComponent> DoorFrame = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door Component");
	TObjectPtr<AActor> DoorHinge = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actor Component|Interaction");
	TObjectPtr<UInteractionComponent> InteractionComponent = nullptr;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnUsed(APawn* Pawn);
	
	UPROPERTY(ReplicatedUsing = OnRep_DoorToggled)
    bool isOpen = false;
    
    UFUNCTION()
	void OnRep_DoorToggled();
	
	UFUNCTION(BlueprintNativeEvent)
	void OpenDoor();
	UFUNCTION(BlueprintNativeEvent)
	void CloseDoor();

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="Target")
	class UMaterialInstanceDynamic* MeshMID;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
