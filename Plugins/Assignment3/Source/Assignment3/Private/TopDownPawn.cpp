// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math.h"

// Sets default values
ATopDownPawn::ATopDownPawn() : MappingContext{nullptr}, MoveAction{nullptr}, ZoomAction{nullptr}, OffsetScaleFactor{50}, ArmLengthScaleFactor{25}
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetCapsuleHalfHeight(25);
	CapsuleComponent->SetCapsuleRadius(25);
	SetRootComponent(CapsuleComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArm->SetupAttachment(CapsuleComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
}

// Called when the game starts or when spawned
void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	MappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* MoveNegateModifier = NewObject<UInputModifierNegate>(this);
	MoveNegateModifier->bX = true;
	MoveNegateModifier->bY = true;
	MoveNegateModifier->bZ = true;

	UInputModifierSwizzleAxis* YXZMoveModifier = NewObject<UInputModifierSwizzleAxis>(this);
	YXZMoveModifier->Order = EInputAxisSwizzle::YXZ;

	FEnhancedActionKeyMapping& ForwardKeyMapping = MappingContext->MapKey(MoveAction, EKeys::W);

	FEnhancedActionKeyMapping& BackwardKeyMapping = MappingContext->MapKey(MoveAction, EKeys::S);
	BackwardKeyMapping.Modifiers.Add(MoveNegateModifier);

	FEnhancedActionKeyMapping& RightKeyMapping = MappingContext->MapKey(MoveAction, EKeys::D);
	RightKeyMapping.Modifiers.Add(YXZMoveModifier);

	FEnhancedActionKeyMapping& LeftKeyMapping = MappingContext->MapKey(MoveAction, EKeys::A);
	LeftKeyMapping.Modifiers.Add(YXZMoveModifier);
	LeftKeyMapping.Modifiers.Add(MoveNegateModifier);

	UInputModifierNegate* ZoomNegateModifier = NewObject<UInputModifierNegate>(this);
	MoveNegateModifier->bX = true;
	MoveNegateModifier->bY = true;
	MoveNegateModifier->bZ = true;

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	FEnhancedActionKeyMapping& ScrollKeyMapping = MappingContext->MapKey(ZoomAction, EKeys::MouseWheelAxis);
	ScrollKeyMapping.Modifiers.Add(ZoomNegateModifier);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Move);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Zoom);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsytem->AddMappingContext(MappingContext,0);
		}
	}
}

void ATopDownPawn::Move(const FInputActionValue& ActionValue) {
	FVector MovementInput = ActionValue.Get<FVector>();

	AddMovementInput(GetActorForwardVector(), MovementInput.X);
	AddMovementInput(GetActorRightVector(), MovementInput.Y);
}

void ATopDownPawn::Zoom(const FInputActionValue& ActionValue) {
	float MouseWheelInput = ActionValue.Get<float>();

	MouseWheelInput = FMath::Clamp(MouseWheelInput, -1.0, 1.0);

	float TempArmLength = SpringArm->TargetArmLength;
	float TempOffset = SpringArm->TargetOffset.Z;

	float CameraPitch = FMath::RadiansToDegrees(atan(TempOffset / TempArmLength));

	if (SpringArm->TargetOffset.Z > 0) {
		if (SpringArm->TargetOffset.Z < 5000) {
			TempOffset += (MouseWheelInput * OffsetScaleFactor);
			TempOffset = FMath::Clamp(TempOffset, 0, 5000);
			SpringArm->TargetOffset.Z = TempOffset;
			CameraPitch = FMath::RadiansToDegrees(atan(TempOffset / TempArmLength));
			Camera->SetRelativeRotation(FRotator{-1.0f * CameraPitch, 0, 0 });

			if (SpringArm->TargetOffset.Z < 1100 && SpringArm->TargetArmLength >= 450 && SpringArm->TargetArmLength <= 600) {
				TempArmLength += (MouseWheelInput * ArmLengthScaleFactor);
				TempArmLength = FMath::Clamp(TempArmLength, 450, 600);
				SpringArm->TargetArmLength = TempArmLength;
			}

			if (SpringArm->TargetOffset.Z > 1100 && SpringArm->TargetOffset.Z < 2350 && SpringArm->TargetArmLength >= 600 && SpringArm->TargetArmLength <= 1200) {
				TempArmLength += (MouseWheelInput * ArmLengthScaleFactor);
				TempArmLength = FMath::Clamp(TempArmLength, 600, 1200);
				SpringArm->TargetArmLength = TempArmLength;
			}
		}
		else if(MouseWheelInput < 1.0) {
			TempOffset += (MouseWheelInput * OffsetScaleFactor);
			TempOffset = FMath::Clamp(TempOffset, 0, 5000);
			SpringArm->TargetOffset.Z = TempOffset;
			CameraPitch = FMath::RadiansToDegrees(atan(TempOffset / TempArmLength));
			Camera->SetRelativeRotation(FRotator{ -1.0f * CameraPitch, 0, 0 });
		}
	}
	else if (SpringArm->TargetArmLength > 250 && SpringArm->TargetArmLength < 500) {
		TempArmLength += (MouseWheelInput * ArmLengthScaleFactor);
		TempArmLength = FMath::Clamp(TempArmLength, 250, 500);
		SpringArm->TargetArmLength = TempArmLength;
	}
	else if (SpringArm->TargetArmLength >= 500) {
		if (MouseWheelInput > 0) {
			TempOffset += (MouseWheelInput * OffsetScaleFactor);
			SpringArm->TargetOffset.Z = TempOffset;
			CameraPitch = FMath::RadiansToDegrees(atan(TempOffset / TempArmLength));
			Camera->SetRelativeRotation(FRotator{-1.0f * CameraPitch, 0, 0 });
		}
		else {
			SpringArm->TargetArmLength += (MouseWheelInput * ArmLengthScaleFactor);
		}
	}
	else if (SpringArm->TargetArmLength <= 250) {
		if (MouseWheelInput > 0) {
			SpringArm->TargetArmLength += (MouseWheelInput * ArmLengthScaleFactor);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentArmLength : %f"), SpringArm->TargetArmLength)
	UE_LOG(LogTemp, Warning, TEXT("CurrentOffset : %f"), SpringArm->TargetOffset.Z)
}