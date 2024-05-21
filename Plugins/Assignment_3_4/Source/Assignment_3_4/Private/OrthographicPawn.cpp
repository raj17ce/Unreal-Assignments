

#include "OrthographicPawn.h"

// Sets default values
AOrthographicPawn::AOrthographicPawn() : OrthographicPawnMappingContext{ nullptr }, CameraMovementAction{ nullptr }, ZoomInOutAction{ nullptr }, CameraXRotationAction{ nullptr } {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeRotation(FRotator(-90.0, 0.0, 0.0));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(SceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AOrthographicPawn::BeginPlay() {
	Super::BeginPlay();
}

void AOrthographicPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrthographicPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	OrthographicPawnMappingContext = NewObject<UInputMappingContext>(this);

	CameraMovementAction = NewObject<UInputAction>(this);
	CameraMovementAction->ValueType = EInputActionValueType::Axis2D;

	ZoomInOutAction = NewObject<UInputAction>(this);
	ZoomInOutAction->ValueType = EInputActionValueType::Axis1D;

	CameraXRotationAction = NewObject<UInputAction>(this);
	CameraXRotationAction->ValueType = EInputActionValueType::Axis1D;

	UInputModifierSwizzleAxis* SwizzleAxisModifierAD = NewObject<UInputModifierSwizzleAxis>(this);
	SwizzleAxisModifierAD->Order = EInputAxisSwizzle::YXZ;
	UInputModifierNegate* MoveActionModifierNegate = NewObject<UInputModifierNegate>(this);

	FEnhancedActionKeyMapping& KeyW = OrthographicPawnMappingContext->MapKey(CameraMovementAction, EKeys::W);

	FEnhancedActionKeyMapping& KeyS = OrthographicPawnMappingContext->MapKey(CameraMovementAction, EKeys::S);
	KeyS.Modifiers.Add(MoveActionModifierNegate);

	FEnhancedActionKeyMapping& KeyD = OrthographicPawnMappingContext->MapKey(CameraMovementAction, EKeys::D);
	KeyD.Modifiers.Add(SwizzleAxisModifierAD);

	FEnhancedActionKeyMapping& KeyA = OrthographicPawnMappingContext->MapKey(CameraMovementAction, EKeys::A);
	KeyA.Modifiers.Add(SwizzleAxisModifierAD);
	KeyA.Modifiers.Add(MoveActionModifierNegate);

	OrthographicPawnMappingContext->MapKey(ZoomInOutAction, EKeys::MouseWheelAxis);
	OrthographicPawnMappingContext->MapKey(CameraXRotationAction, EKeys::MouseX);

	if (EIC) {
		EIC->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleCameraMovement);
		EIC->BindAction(ZoomInOutAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleCameraZoom);
		EIC->BindAction(CameraXRotationAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleRotationX);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			SubSystem->AddMappingContext(OrthographicPawnMappingContext, 0);
		}
	}
}

void AOrthographicPawn::HandleCameraMovement(const FInputActionValue& ActionValue) {
	FVector2D MovementScale = ActionValue.Get<FVector2D>();

	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetActorRotation()), MovementScale.X);
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetActorRotation()), MovementScale.Y);
}

void AOrthographicPawn::HandleCameraZoom(const FInputActionValue& ActionValue) {
	float ZoomScale = ActionValue.Get<float>();

	SpringArm->TargetArmLength -= ZoomScale * 25;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 250.0f, FLT_MAX);
}

void AOrthographicPawn::HandleRotationX(const FInputActionValue& ActionValue) {
	float RotateScale = ActionValue.Get<float>();

	AddActorLocalRotation(FRotator{ 0.0, RotateScale, 0.0 });
}