// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DefaultPlayerController.h"
#include "Camera/CameraComponent.h"

#include "DefaultPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SummerProject/ActorComponents/InteractableComponent/InteractionComponent.h"

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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
<<<<<<< HEAD
	CharacterInputComponent = PlayerInputComponent;
	LocalPlayer = Cast<ADefaultPlayerController>(Controller)->GetLocalPlayer();
	
}
=======
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	DefaultPlayerController = Cast<ADefaultPlayerController>(Controller);
	
	check(EnhancedInputComponent && DefaultPlayerController);
	BIND_ACTION_IF_VALID(ActionMove, ETriggerEvent::Triggered, &APlayerCharacter::HandleMove);
	//if(DefaultPlayerController->ActionMove){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionMove, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMove);}
	if(DefaultPlayerController->ActionLook){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionLook, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLook);}
	if(DefaultPlayerController->ActionJump){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionJump, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleJump);}
	if(DefaultPlayerController->ActionRun){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionRun, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleRun);}
	BIND_ACTION_IF_VALID(ActionUse, ETriggerEvent::Triggered, &APlayerCharacter::HandleUse);
	
	ULocalPlayer* LocalPlayer = DefaultPlayerController->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(DefaultPlayerController->InputMappingContext, 0);
}

void APlayerCharacter::InteractionLineTrace(int16 TraceDistance)
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
				Component->Used(this);
			}
		}
	}
}

void APlayerCharacter::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2d MovementVector = IAVal.Get<FVector2d>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::HandleLook(const FInputActionValue& IAVal)
{
	const FVector2d LookVector = IAVal.Get<FVector2d>();
	DefaultPlayerController->AddYawInput(LookVector.X);
	DefaultPlayerController->AddPitchInput(LookVector.Y);
}

void APlayerCharacter::HandleJump()
{
	Jump();
}

void APlayerCharacter::HandleRun()
{
	printf("RUN");
}

void APlayerCharacter::HandleUse()
{
	InteractionLineTrace(InteractionRange);
}

>>>>>>> recover
