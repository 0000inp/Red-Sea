#include "SubmarineController.h"


#include "Components/ArrowComponent.h"
#include "SummerProject/Dev/DEBUG.h"
#include "SummerProject/Player/PlayerCharacter.h"
#include "SummerProject/Player/Submarine/Submarine.h"


#define BIND_ACTION_IF_VALID(Action, TriggerEvent, Callback) \
	do { \
		if ((DefaultPlayerController) && (DefaultPlayerController)->Action) { \
		EnhancedInputComponent->BindAction((DefaultPlayerController)->Action, TriggerEvent, this, Callback); \
		} \
	} while (0)

ASubmarineController::ASubmarineController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = Root;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	Mesh->SetupAttachment(RootComponent);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ASubmarineController::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent->onInteract.AddDynamic(this, &ASubmarineController::ControllerInteracted);
}

// Called every frame
void ASubmarineController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASubmarineController::ControllerInteracted(APlayerCharacter* Player)
{
	if(isBeingUse == false && Player)
	{
		Player->UseSubmarineController(Cast<ISubmarineControl>(this));
		BeingUse();
	}
}


void ASubmarineController::ControlMovement(FVector2d MoveVector)
{
	float speed = MoveVector.Y/10;
	MoveSubmarine(speed);
}

void ASubmarineController::MoveSubmarine_Implementation(float speed)
{
	Submarine->MoveValue.Horizontal += speed;
}



void ASubmarineController::ControlRotation(FVector2d LookVector)
{
	FRotator RotationToAdd = {LookVector.Y,LookVector.X,0};
	
	RotateSubmarine(RotationToAdd);
}

void ASubmarineController::RotateSubmarine_Implementation(FRotator Rotator)
{
	Submarine->ArrowComponent->AddLocalRotation(Rotator);
}


void ASubmarineController::BeingUse()
{
	isBeingUse = true;
}

void ASubmarineController::StopBeingUse()
{
	isBeingUse = false;
}








