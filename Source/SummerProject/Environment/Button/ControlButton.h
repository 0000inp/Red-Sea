#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"

#include "ControlButton.generated.h"

UCLASS()
class SUMMERPROJECT_API AControlButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AControlButton();
	
	UPROPERTY(EditAnywhere);
	TObjectPtr<USceneComponent> Root = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actor Component|Interaction");
	TObjectPtr<UInteractionComponent> InteractionComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bIsPressed = false;
	
	UFUNCTION(BlueprintCallable)
	void ButtonInteracted(APlayerCharacter* Player);
	
	UFUNCTION(BlueprintCallable)
	void ButtonPressed(APlayerCharacter* Player);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void onButtonActivation();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};