// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DefaultPlayerController.h"
#include "Camera/CameraComponent.h"

#include "DefaultPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	DefaultPlayerController = Cast<ADefaultPlayerController>(Controller);

	check(EnhancedInputComponent && DefaultPlayerController);
	if(DefaultPlayerController->ActionMove){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionMove, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMove);}
	if(DefaultPlayerController->ActionLook){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionLook, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLook);}
	if(DefaultPlayerController->ActionJump){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionJump, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleJump);}
	if(DefaultPlayerController->ActionRun){EnhancedInputComponent->BindAction(DefaultPlayerController->ActionRun, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleRun);}

	ULocalPlayer* LocalPlayer = DefaultPlayerController->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(DefaultPlayerController->InputMappingContext, 0);
}


void APlayerCharacter::HandleMove(const FInputActionValue& IAVal)
{
	const FVector2d MovementVector = IAVal.Get<FVector2d>();
	this->AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	this->AddMovementInput(GetActorRightVector(), MovementVector.X);
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

