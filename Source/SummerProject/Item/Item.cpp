// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"
#include "SummerProject/Dev/DEBUG.h"
#include "SummerProject/Player/PlayerCharacter.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	//RootComponent = Root;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	//Mesh->SetupAttachment(RootComponent);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent->OnUsed.AddDynamic(this, &AItem::PickedUp);
	SimulatePhysic(true);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AItem::PickedUp(APlayerCharacter* Player)
{
	OwnerPlayer = Player;
	OwnerPlayer->InventoryComponent->SetHoldingItem(this);
	
	SimulatePhysic(false);
	AttachToComponent(OwnerPlayer->ItemPlaceholder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


void AItem::Dropped()
{
	SimulatePhysic(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}



void AItem::SimulatePhysic(bool simulate)
{
	Mesh->SetSimulatePhysics(simulate);
	Multicast_SimulatePhysic(simulate);
}

void AItem::Multicast_SimulatePhysic_Implementation(bool simulate)
{
	Mesh->SetSimulatePhysics(simulate);
}


