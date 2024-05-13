// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = Root;
	
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	DoorFrame->SetupAttachment(Root);
	
	
	DoorHinge = CreateDefaultSubobject<AActor>(TEXT("Door Hinge"));
	
	DoorPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Panel"));
	DoorPanel->SetupAttachment(Root);
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent->OnUsed.AddDynamic(this, &ADoor::OnUsed);
	
	MeshMID = DoorPanel->CreateDynamicMaterialInstance(0);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::OnUsed(APawn* Pawn)
{
	if(HasAuthority())
	{
		isOpen = !isOpen;
		OnRep_DoorToggled();
	}
}

void ADoor::OnRep_DoorToggled()
{
	if(isOpen){OpenDoor();}
	else{CloseDoor();}
}

void ADoor::OpenDoor_Implementation()
{
	DoorPanel->AddWorldOffset(FVector::UpVector * 20);
}

void ADoor::CloseDoor_Implementation()
{
	DoorPanel->AddWorldOffset(FVector::UpVector * -20);
}


void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ADoor, isOpen)
}



