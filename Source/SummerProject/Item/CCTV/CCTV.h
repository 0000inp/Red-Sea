// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SummerProject/Item/Item.h"
#include "CCTV.generated.h"

UCLASS()
class SUMMERPROJECT_API ACCTV : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACCTV();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> PlacingExampleMesh = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlaceTraceDistance = 1000.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//UFUNCTION(BlueprintCallable)
	//void LineTracePlaceLocation();
	
};
