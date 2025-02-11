// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <string>

#include "DefaultPlayerController.h"
#include "Camera/CameraComponent.h"

#include "DefaultPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UnrealWidgetFwd.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"
#include "SummerProject/Item/Item.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/PhysicsVolume.h"


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
	
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->SetupAttachment(Camera);
	
	MovementComponent->MaxSwimSpeed = SwimSpeed;
	
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

	ResourceCalculation(DeltaTime);
	
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
	BIND_ACTION_IF_VALID(ActionRun, ETriggerEvent::Triggered, &APlayerCharacter::HandleRun);
	BIND_ACTION_IF_VALID(ActionUse, ETriggerEvent::Triggered, &APlayerCharacter::HandleUse);
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
/*
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
}*/

void APlayerCharacter::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2D MovementVector = IAVal.Get<FVector2D>();

	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	FVector RightDirection = GetActorRightVector();

	const TEnumAsByte<enum EMovementMode> MovementMode = GetCharacterMovement()->MovementMode;
	
	float SpeedModifier = 1.0f;
	
	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		SpeedModifier = 1;
	}
	if (MovementMode == EMovementMode::MOVE_Swimming)
	{
		static float TimeInMove = 0.0f;
		TimeInMove += GetWorld()->GetDeltaSeconds();
	
		if (SwimMovementCurve)
		{
			SpeedModifier = SwimMovementCurve->GetFloatValue(TimeInMove * MovementSpeedFrequency) * MovementSpeedAmplitude;
			SpeedModifier = FMath::Abs(SpeedModifier);
		}
	
		SpeedModifier = FMath::Clamp(SpeedModifier, 0.01f, MovementSpeedAmplitude);
	}
	
	AddMovementInput(ForwardDirection, MovementVector.Y * SpeedModifier);
	AddMovementInput(RightDirection, MovementVector.X * SpeedModifier);
	
	/*if (MovementVector.IsNearlyZero())
	{
		SpeedModifier = FMath::FInterpTo(SpeedModifier, 0.0f, GetWorld()->GetDeltaSeconds(), 0.5f); 
	}*/
}


void APlayerCharacter::HandleLook(const FInputActionValue& IAVal)
{
	const FVector2D LookVector = IAVal.Get<FVector2D>();
	MouseMovementInputValue = LookVector;
	
	DefaultPlayerController->AddYawInput(LookVector.X);
	DefaultPlayerController->AddPitchInput(LookVector.Y);
}

void APlayerCharacter::HandleJump(const FInputActionValue& IAVal)
{
	const bool bIsPressed = IAVal.Get<bool>();

	const TEnumAsByte<enum EMovementMode> MovementMode = GetCharacterMovement()->MovementMode;
	
	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		Jump();
	}
	
	if (MovementMode == EMovementMode::MOVE_Swimming)
	{
		FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(GetControlRotation());
		LaunchCharacter(ForwardDirection * MaxStamina,true,true);
	}
}

void APlayerCharacter::HandleRun(const FInputActionValue& IAVal)
{
	const bool bIsPressed = IAVal.Get<bool>();

	if (bIsPressed)
	{
		MovementComponent->MaxSwimSpeed = RunSpeed;
		MovementSpeedFrequency = FastMovementSpeedFrequency;
		if (GetVelocity().Length() >= 350){bIsRunning = true;}
		else{bIsRunning = false;}
	}
	else
	{
		MovementComponent->MaxSwimSpeed = SwimSpeed;
		MovementSpeedFrequency = NormalMovementSpeedFrequency;
		bIsRunning = false;
	}
	
}

void APlayerCharacter::HandleUse(const FInputActionValue& IAVal)
{
	const bool bIsPressed = IAVal.Get<bool>();
	
	if (bIsPressed){WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);}
	else{WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);}
	
	if(HasAuthority())
	{
		InteractWithInteractable(LookingAtInteractionComponent, bIsPressed);
	}
	else{Server_InteractWithInteractable(LookingAtInteractionComponent, bIsPressed);}
	
}

void APlayerCharacter::InteractWithInteractable(UInteractionComponent* Component, bool bIsInteract)
{
	if(Component)
	{
		if(bIsInteract){ Component->Interact(this); }
		else{ Component->StopInteract(this); }
	}
	
}


void APlayerCharacter::Server_InteractWithInteractable_Implementation(UInteractionComponent* Component, bool bIsInteract)
{
	if(Component)
	{
		if(bIsInteract){ Component->Interact(this); }
		else{ Component->StopInteract(this); }
	}
}

void APlayerCharacter::HandleDropItem(const FInputActionValue& IAVal)
{
	const bool bIsPressed = IAVal.Get<bool>();
	
	InventoryComponent->DropItem();
}

void APlayerCharacter::ResourceCalculation(float DeltaTime)
{
	if (bIsInWater)
	{
		if (bIsRunning)
		{
			Stamina -= StaminaDepletionRate * DeltaTime;
		}
		else
		{
			Stamina += StaminaIncreaseRate * DeltaTime;
		}
		float StaminaEffect = 1 + ((((MaxStamina - Stamina)/MaxStamina) * StaminaResourceDepletionScale));
		Oxygen -= OxygenDepletionRate * DeltaTime * StaminaEffect;
	}
	
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
	Oxygen = FMath::Clamp(Oxygen, 0.0f, MaxOxygen);
}

void APlayerCharacter::InWaterMode()
{
	bIsInWater = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
}

void APlayerCharacter::OutWaterMode()
{
	bIsInWater = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
}





