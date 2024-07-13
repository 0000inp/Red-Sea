
#include "ControlButton.h"
#include "SummerProject/Dev/DEBUG.h"

AControlButton::AControlButton()
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
void AControlButton::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent->onInteract.AddDynamic(this, &AControlButton::ButtonInteracted);
}

// Called every frame
void AControlButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AControlButton::ButtonInteracted(APlayerCharacter* Player)
{
	ButtonPressed(Player);
}

void AControlButton::ButtonPressed(APlayerCharacter* Player)
{
	onButtonActivation();
	bIsPressed = true;
}



