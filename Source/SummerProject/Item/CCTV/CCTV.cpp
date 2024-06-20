// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTV.h"

#include "Camera/CameraComponent.h"
#include "SummerProject/Player/PlayerCharacter.h"


// Sets default values
ACCTV::ACCTV()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACCTV::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FHitResult Hit;
	//FVector TraceStart = OwnerPlayer->Camera->GetComponentLocation();
	//FVector TraceEnd = TraceStart + (OwnerPlayer->Camera->GetForwardVector() * PlaceTraceDistance);

	/*if(GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1))
	{
		if(Hit.GetActor()->IsValidLowLevelFast())
		{
			TInlineComponentArray<UInteractionComponent*> Components;
			Hit.GetActor()->GetComponents(Components);
			for(UInteractionComponent* Component : Components)
			{
				if(HasAuthority())
				{
					Component->Used(this);
				}
				else{Server_UseInteractable(Component);}
			}
		}
	}*/
}

