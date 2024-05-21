// Fill out your copyright notice in the Description page of Project Settings.


#include "PerspectivePawn.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APerspectivePawn::APerspectivePawn() : PerspectivePawnMapping{ nullptr }, MoveAction{ nullptr }, LookAction{ nullptr }, MoveScale{ 1.0f } {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(SceneRoot);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void APerspectivePawn::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void APerspectivePawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APerspectivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	PerspectivePawnMapping = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;

	UInputModifierSwizzleAxis* SwizzleAxisModifierAD = NewObject<UInputModifierSwizzleAxis>(this);
	SwizzleAxisModifierAD->Order = EInputAxisSwizzle::YXZ;
	UInputModifierSwizzleAxis* SwizzleAxisModifierQE = NewObject<UInputModifierSwizzleAxis>(this);
	SwizzleAxisModifierQE->Order = EInputAxisSwizzle::ZYX;
	UInputModifierNegate* MoveActionModifierNegate = NewObject<UInputModifierNegate>(this);

	FEnhancedActionKeyMapping& KeyW = PerspectivePawnMapping->MapKey(MoveAction, EKeys::W);

	FEnhancedActionKeyMapping& KeyS = PerspectivePawnMapping->MapKey(MoveAction, EKeys::S);
	KeyS.Modifiers.Add(MoveActionModifierNegate);

	FEnhancedActionKeyMapping& KeyD = PerspectivePawnMapping->MapKey(MoveAction, EKeys::D);
	KeyD.Modifiers.Add(SwizzleAxisModifierAD);

	FEnhancedActionKeyMapping& KeyA = PerspectivePawnMapping->MapKey(MoveAction, EKeys::A);
	KeyA.Modifiers.Add(SwizzleAxisModifierAD);
	KeyA.Modifiers.Add(MoveActionModifierNegate);

	FEnhancedActionKeyMapping& KeyE = PerspectivePawnMapping->MapKey(MoveAction, EKeys::E);
	KeyE.Modifiers.Add(SwizzleAxisModifierQE);

	FEnhancedActionKeyMapping& KeyQ = PerspectivePawnMapping->MapKey(MoveAction, EKeys::Q);
	KeyQ.Modifiers.Add(SwizzleAxisModifierQE);
	KeyQ.Modifiers.Add(MoveActionModifierNegate);

	LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* LookActionModifierNegate = NewObject<UInputModifierNegate>(this);
	LookActionModifierNegate->bX = false;
	LookActionModifierNegate->bY = true;
	LookActionModifierNegate->bZ = false;

	FEnhancedActionKeyMapping& LookActionMouseMapping = PerspectivePawnMapping->MapKey(LookAction, EKeys::Mouse2D);
	LookActionMouseMapping.Modifiers.Add(LookActionModifierNegate);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectivePawn::HandleMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APerspectivePawn::HandleLook);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	check(PlayerController)

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(PerspectivePawnMapping, 0);
}

void APerspectivePawn::HandleMove(const FInputActionValue& ActionValue) {
	FVector MovementInput = ActionValue.Get<FVector>();

	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), MovementInput.X * MoveScale);
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), MovementInput.Y * MoveScale);
	AddMovementInput(UKismetMathLibrary::GetUpVector(GetActorRotation()), MovementInput.Z * MoveScale);
}

void APerspectivePawn::HandleLook(const FInputActionValue& ActionValue) {
	FVector2D LookAxisValue = ActionValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}