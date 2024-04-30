// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math.h"

// Sets default values
ATopDownPawn::ATopDownPawn() : MappingContext{nullptr}, MoveAction{nullptr}, ZoomAction{nullptr}
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetCapsuleHalfHeight(25);
	CapsuleComponent->SetCapsuleRadius(25);
	SetRootComponent(CapsuleComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArm->TargetArmLength = 700;
	SpringArm->SetRelativeRotation(FRotator(-50,0,0));
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

	float CurrentArmLength = SpringArm->TargetArmLength;
	float CurrentYZLength = abs(MouseWheelInput * 25);
	float NewPitch = (asin(CurrentYZLength / CurrentArmLength) * 180 / PI);

	UE_LOG(LogTemp, Warning, TEXT("CurrentArmLength : %f"), CurrentArmLength)

	if (SpringArm->TargetArmLength > 400.0) {
		SpringArm->TargetArmLength += (MouseWheelInput * 50);
		//UE_LOG(LogTemp, Warning, TEXT("TargetArmLength1 : %f"), SpringArm->TargetArmLength)
	}
	else if(SpringArm->TargetOffset.Z > -300.0) {

		UE_LOG(LogTemp, Warning, TEXT("NewPitch : %f"), NewPitch)

		if (SpringArm->TargetOffset.Z == 0.0) {
			if (MouseWheelInput < 0.0) {
				SpringArm->TargetOffset.Z += (MouseWheelInput * 25);
				Camera->AddRelativeRotation(FRotator(NewPitch, 0, 0));
			}
			else {
				SpringArm->TargetArmLength += (MouseWheelInput * 50);
			}
		}
		else if (SpringArm->TargetOffset.Z < 0.0) {
			SpringArm->TargetOffset.Z += (MouseWheelInput * 25);
			Camera->AddRelativeRotation(FRotator(MouseWheelInput * -1.0f * NewPitch, 0, 0));
		}

		UE_LOG(LogTemp, Warning, TEXT("SpringArm->TargetOffset.Z : %f"), SpringArm->TargetOffset.Z)
		UE_LOG(LogTemp, Warning, TEXT("Camera Pitch : %f"), Camera->GetRelativeRotation().Pitch)
	}
	else if (SpringArm->TargetOffset.Z == -300.0 && MouseWheelInput > 0.0) {
		SpringArm->TargetOffset.Z += (MouseWheelInput * 25);
		Camera->AddRelativeRotation(FRotator(-1.0f * NewPitch, 0, 0));
	}
}