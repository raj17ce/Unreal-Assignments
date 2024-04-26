// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

AFirstPersonPawn::AFirstPersonPawn() : PawnMappingContext{ nullptr }, MoveAction{ nullptr }, LookAction{ nullptr } {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetCapsuleHalfHeight(90);
	CapsuleComponent->SetCapsuleRadius(45);
	//CapsuleComponent->SetEnableGravity(true);
	//CapsuleComponent->SetSimulatePhysics(true);

	SetRootComponent(CapsuleComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(CapsuleComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
}

// Called when the game starts or when spawned
void AFirstPersonPawn::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AFirstPersonPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	PawnMappingContext = NewObject<UInputMappingContext>(this, TEXT("Pawn Mapping Context"));

	LookAction = NewObject<UInputAction>(this, TEXT("Look Action"));
	LookAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* LookNegateModifier = NewObject<UInputModifierNegate>(this, TEXT("Look Negate Modifier"));
	LookNegateModifier->bX = false;
	LookNegateModifier->bY = true;
	LookNegateModifier->bZ = false;

	FEnhancedActionKeyMapping& LookAroundKeyMapping = PawnMappingContext->MapKey(LookAction, EKeys::Mouse2D);
	LookAroundKeyMapping.Modifiers.Add(LookNegateModifier);

	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Look);

	MoveAction = NewObject<UInputAction>(this, "Move Action");
	MoveAction->ValueType = EInputActionValueType::Axis3D;

	UInputModifierNegate* MoveNegateModifier = NewObject<UInputModifierNegate>(this, TEXT("Move Negate Modifier"));
	MoveNegateModifier->bX = true;
	MoveNegateModifier->bY = true;
	MoveNegateModifier->bZ = true;

	UInputModifierSwizzleAxis* YXZMoveModifier = NewObject<UInputModifierSwizzleAxis>(this, TEXT("YXZ Move Modifier"));
	YXZMoveModifier->Order = EInputAxisSwizzle::YXZ;

	UInputModifierSwizzleAxis* ZYXMoveModifier = NewObject<UInputModifierSwizzleAxis>(this, TEXT("ZYX Move Modifier"));
	ZYXMoveModifier->Order = EInputAxisSwizzle::ZYX;

	FEnhancedActionKeyMapping& MoveForwardKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::W);

	FEnhancedActionKeyMapping& MoveBackwardKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::S);
	MoveBackwardKeyMapping.Modifiers.Add(MoveNegateModifier);

	FEnhancedActionKeyMapping& MoveRightKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::D);
	MoveRightKeyMapping.Modifiers.Add(YXZMoveModifier);

	FEnhancedActionKeyMapping& MoveLeftKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::A);
	MoveLeftKeyMapping.Modifiers.Add(YXZMoveModifier);
	MoveLeftKeyMapping.Modifiers.Add(MoveNegateModifier);

	FEnhancedActionKeyMapping& MoveUpKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::E);
	MoveUpKeyMapping.Modifiers.Add(ZYXMoveModifier);

	FEnhancedActionKeyMapping& MoveDownKeyMapping = PawnMappingContext->MapKey(MoveAction, EKeys::Q);
	MoveDownKeyMapping.Modifiers.Add(ZYXMoveModifier);
	MoveDownKeyMapping.Modifiers.Add(MoveNegateModifier);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Move);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(PawnMappingContext, 0);
		}
	}
}

void AFirstPersonPawn::Look(const FInputActionValue& ActionValue) {
	FVector2D AxisInput = ActionValue.Get<FInputActionValue::Axis2D>();

	AddControllerYawInput(AxisInput.X);
	AddControllerPitchInput(AxisInput.Y);
}

void AFirstPersonPawn::Move(const FInputActionValue& ActionValue) {
	FVector MovementInput = ActionValue.Get<FVector>();

	AddMovementInput(GetControlRotation().RotateVector(MovementInput));
}