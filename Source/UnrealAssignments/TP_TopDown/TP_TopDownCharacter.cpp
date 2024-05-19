// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ATP_TopDownCharacter::ATP_TopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	PawnAttributeAsset = LoadObject<UPawnAttributeAsset>(this, TEXT("/Script/Assignment_1_2.PawnAttributeAsset'/Assignment_1_2/Data/BP_TDPAsset.BP_TDPAsset'"));
}

void ATP_TopDownCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATP_TopDownCharacter::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATP_TopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATP_TopDownCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATP_TopDownCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATP_TopDownCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATP_TopDownCharacter::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ATP_TopDownCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ATP_TopDownCharacter::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ATP_TopDownCharacter::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ATP_TopDownCharacter::OnTouchReleased);
	}
}

void ATP_TopDownCharacter::OnInputStarted() {
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	PlayerController->StopMovement();
}

void ATP_TopDownCharacter::OnSetDestinationTriggered() {
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch) {
		bHitSuccessful = PlayerController->GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else {
		bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful) {
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = PlayerController->GetPawn();
	if (ControlledPawn != nullptr) {
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ATP_TopDownCharacter::OnSetDestinationReleased() {
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	// If it was a short press
	if (FollowTime <= ShortPressThreshold) {
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(PlayerController, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void ATP_TopDownCharacter::OnTouchTriggered() {
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ATP_TopDownCharacter::OnTouchReleased() {
	bIsTouch = false;
	OnSetDestinationReleased();
}