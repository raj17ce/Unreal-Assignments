// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricPawn.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"

// Sets default values
AIsometricPawn::AIsometricPawn() : IsometricPawnMappingContext{ nullptr }, CameraLeftRotationAction{ nullptr }, CameraRightRotationAction{ nullptr }, ZoomInOutAction{ nullptr }, RotationSpeed{ 2.0f }, TargetRotationYawValue{ 45.0f }  {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	SpringArm->SetRelativeRotation(FRotator{ -45.0, 45.0, 0.0 });
	SpringArm->TargetArmLength = 2500;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->CameraRotationLagSpeed = 30.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));
	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);
	Camera->AddRelativeRotation(FRotator{ 0, 0, 0 });

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

// Called when the game starts or when spawned
void AIsometricPawn::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AIsometricPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotationYawValue, CurrentRotation.Roll);
	SpringArm->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime));
}

// Called to bind functionality to input
void AIsometricPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	IsometricPawnMappingContext = NewObject<UInputMappingContext>(this);

	CameraLeftRotationAction = NewObject<UInputAction>(this);
	CameraLeftRotationAction->ValueType = EInputActionValueType::Boolean;

	IsometricPawnMappingContext->MapKey(CameraLeftRotationAction, EKeys::Q);

	CameraRightRotationAction = NewObject<UInputAction>(this);
	CameraRightRotationAction->ValueType = EInputActionValueType::Boolean;

	IsometricPawnMappingContext->MapKey(CameraRightRotationAction, EKeys::E);

	ZoomInOutAction = NewObject<UInputAction>(this);
	ZoomInOutAction->ValueType = EInputActionValueType::Axis1D;

	IsometricPawnMappingContext->MapKey(ZoomInOutAction, EKeys::MouseWheelAxis);

	if (EIC) {
		EIC->BindAction(CameraLeftRotationAction, ETriggerEvent::Completed, this, &AIsometricPawn::HandleCameraLeftRotation);
		EIC->BindAction(CameraRightRotationAction, ETriggerEvent::Completed, this, &AIsometricPawn::HandleCameraRightRotation);
		EIC->BindAction(ZoomInOutAction, ETriggerEvent::Triggered, this, &AIsometricPawn::HandleCameraZoom);
	}

	if (auto* PlayerController = Cast<APlayerController>(GetController())) {
		if (auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			SubSystem->AddMappingContext(IsometricPawnMappingContext, 0);
		}
	}
}

void AIsometricPawn::HandleCameraLeftRotation() {
	TargetRotationYawValue += 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricPawn::HandleCameraRightRotation() {
	TargetRotationYawValue -= 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricPawn::HandleCameraZoom(const FInputActionValue& ActionValue) {
	float value = ActionValue.Get<float>();

	SpringArm->TargetArmLength -= value * 50;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 1500, 3000);
}