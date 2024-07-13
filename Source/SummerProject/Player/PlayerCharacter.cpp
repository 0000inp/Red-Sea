// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DefaultPlayerController.h"
#include "Camera/CameraComponent.h"

#include "DefaultPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"
#include "SummerProject/Item/Item.h"

#include "SummerProject/Dev/DEBUG.h"
#include "SummerProject/Interface/SubmarineControl.h"

class UInteractionComponent;

#define BIND_ACTION_IF_VALID(Action, TriggerEvent, Callback) \
	do { \
		if ((DefaultPlayerController) && (DefaultPlayerController)->Action) { \
			EnhancedInputComponent->BindAction((DefaultPlayerController)->Action, TriggerEvent, this, Callback); \
		} \
	} while (0)


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = GetCharacterMovement();

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	ItemPlaceholder = CreateDefaultSubobject<USceneComponent>(TEXT("Item Placeholder"));
	ItemPlaceholder->SetupAttachment(Camera);
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	SetupStimulusSource();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("---------%s-------->>>>> is LocallyControlled = %hs"),*GetName(), IsLocallyControlled() ? "true" : "false")
	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	GetWorld()->Exec(GetWorld(), TEXT("viewmode unlit"));
	Multicast_UnlitMode();
}
void APlayerCharacter::Multicast_UnlitMode_Implementation(){GetWorld()->Exec(GetWorld(), TEXT("viewmode unlit"));}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookingAtInteractionComponent = InteractionLineTrace(InteractionRange);
	
}

//Setup Stimulus source
void APlayerCharacter::SetupStimulusSource()
{
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stumuli Source"));
	if(StimuliSourceComponent)
	{
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	DefaultPlayerController = Cast<ADefaultPlayerController>(Controller);

	ULocalPlayer* LocalPlayer = DefaultPlayerController->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(DefaultPlayerController->InputMappingContext, 0);
	
	check(EnhancedInputComponent && DefaultPlayerController);
	
	BIND_ACTION_IF_VALID(ActionMove, ETriggerEvent::Triggered, &APlayerCharacter::HandleMove);
	BIND_ACTION_IF_VALID(ActionLook, ETriggerEvent::Triggered, &APlayerCharacter::HandleLook);
	BIND_ACTION_IF_VALID(ActionJump, ETriggerEvent::Triggered, &APlayerCharacter::HandleJump);
	BIND_ACTION_IF_VALID(ActionRun, ETriggerEvent::Ongoing, &APlayerCharacter::HandleRun);
	BIND_ACTION_IF_VALID(ActionUse, ETriggerEvent::Triggered, &APlayerCharacter::HandleUse);
	BIND_ACTION_IF_VALID(ActionUse, ETriggerEvent::Canceled, &APlayerCharacter::HandleStopUse);
	BIND_ACTION_IF_VALID(ActionDropItem, ETriggerEvent::Started, &APlayerCharacter::HandleDropItem);
	
}

UInteractionComponent* APlayerCharacter::InteractionLineTrace(int16 TraceDistance)
{
	FHitResult Hit;
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * TraceDistance);
	if(GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1))
	{
		if(Hit.GetActor()->IsValidLowLevelFast())
		{
			TInlineComponentArray<UInteractionComponent*> Components;
			Hit.GetActor()->GetComponents(Components);
			for(UInteractionComponent* Component : Components)
			{
				return Component;
			}
		}
	}
	return nullptr;
}

void APlayerCharacter::UseInteractable(UInteractionComponent* Component)
{
	if(Component)
	{
		/*if(ISubmarineControl* Interface = Cast<ISubmarineControl>(Component->GetOwner()))
		{
			SubmarineControlInterface = Interface;
			DEBUG::print("Start Control");
		}*/
		Component->Interact(this);
	}
	
}


void APlayerCharacter::Server_UseInteractable_Implementation(UInteractionComponent* Component)
{
	if(Component){Component->Interact(this);}
}

void APlayerCharacter::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2d MovementVector = IAVal.Get<FVector2d>();
	
	if(SubmarineControlInterface)
	{
		SubmarineControlInterface->ControlMovement(MovementVector);
		return;
	}
	
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::HandleLook(const FInputActionValue& IAVal)
{
	const FVector2d LookVector = IAVal.Get<FVector2d>();
	
	if(SubmarineControlInterface)
	{
		SubmarineControlInterface->ControlRotation(LookVector);
		return;
	}
	
	DefaultPlayerController->AddYawInput(LookVector.X);
	DefaultPlayerController->AddPitchInput(LookVector.Y);
}

void APlayerCharacter::HandleJump()
{
	Jump();
}

void APlayerCharacter::HandleRun()
{
	MovementComponent->MaxWalkSpeed = 1000;
	HealthComponent->Dead();
}

void APlayerCharacter::HandleUse()
{
	if(HasAuthority())
	{
		UseInteractable(LookingAtInteractionComponent);
	}
	else{Server_UseInteractable(LookingAtInteractionComponent);}
	
}

void APlayerCharacter::HandleStopUse()
{
	DEBUG::print("HandleStopUse STOP INTERACTING");
}


void APlayerCharacter::HandleDropItem()
{
	InventoryComponent->DropItem();

	if(SubmarineControlInterface){StopUseSubmarineController();  DEBUG::print("Stop Control");}
}


void APlayerCharacter::UseSubmarineController(ISubmarineControl* ControlInterface)
{
	if(SubmarineControlInterface == nullptr)
	{
		SubmarineControlInterface = ControlInterface;
	}
}

void APlayerCharacter::StopUseSubmarineController()
{
	if(SubmarineControlInterface)
	{
		SubmarineControlInterface->StopBeingUse();
		SubmarineControlInterface = nullptr;
	}
}





