// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicShapeController.h"


ADynamicShapeController::ADynamicShapeController() : bIsActorMovable{true}, Widget{ nullptr }, SpawnedActor{nullptr}, ShapeType{EShapeType::Spherical} {}


void ADynamicShapeController::BeginPlay() {
	Super::BeginPlay();

	if (WidgetClass) {
		Widget = CreateWidget<UMeshGenerationUI>(this, WidgetClass, "Widget");
		Widget->AddToViewport(1);

		SetShowMouseCursor(true);
		SetInputMode(InputMode);

		Widget->BoxDimensions->SetVisibility(ESlateVisibility::Hidden);

		Widget->ShapeType->OnSelectionChanged.AddDynamic(this, &ADynamicShapeController::HandleShapeTypeChange);
		Widget->SphereRadius->OnValueChanged.AddDynamic(this, &ADynamicShapeController::HandleSphereRadiusChange);
		Widget->BoxDimensionX->OnValueChanged.AddDynamic(this, &ADynamicShapeController::HandleBoxDimensionChange);
		Widget->BoxDimensionY->OnValueChanged.AddDynamic(this, &ADynamicShapeController::HandleBoxDimensionChange);
		Widget->BoxDimensionZ->OnValueChanged.AddDynamic(this, &ADynamicShapeController::HandleBoxDimensionChange);

		SpawnedActor = GetWorld()->SpawnActor<ASelectionArea>(FVector::ZeroVector, FRotator::ZeroRotator);

		GenerateNewSphere(250.0f);
	} 
}

void ADynamicShapeController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsActorMovable) {
		HandleActorLocationChange();
	}
}

void ADynamicShapeController::SetupInputComponent() {

	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		auto* MappingContext = NewObject<UInputMappingContext>();

		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &ADynamicShapeController::HandleLeftMouseClick);

		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ADynamicShapeController::HandleShapeTypeChange(FString SelectedItem, ESelectInfo::Type SelectionType) {
	if (Widget) {
		if (ASelectionArea::GetShapeTypeFromString(SelectedItem) == EShapeType::Spherical) {
			ShapeType = EShapeType::Spherical;
			Widget->HideBoxDimensions();

			GenerateNewSphere(250.0f);
		}
		else if (ASelectionArea::GetShapeTypeFromString(SelectedItem) == EShapeType::Box) {
			ShapeType = EShapeType::Box;
			Widget->HideSphereRadius();

			GenerateNewBox(500.0f);
		}
	}
}

void ADynamicShapeController::GenerateNewSphere(float NewRadius) {
	float Radius = Widget->SphereRadius->GetValue();
	SpawnedActor->GenerateSphere(0, Radius, FMath::FloorToInt(Radius / 10), FMath::FloorToInt(Radius / 5), Radius);
}

void ADynamicShapeController::GenerateNewBox(float NewValue) {
	FVector BoxDimensions{ Widget->BoxDimensionX->GetValue(), Widget->BoxDimensionY->GetValue(), Widget->BoxDimensionZ->GetValue() };
	SpawnedActor->GenerateCube(0, BoxDimensions, BoxDimensions.Z / 2);
}

void ADynamicShapeController::HandleActorLocationChange() {
	
	FVector WorldLocation;
	FVector WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(SpawnedActor);

		FHitResult HitResult;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams)) {
			if (HitResult.bBlockingHit) {
				HitResult.Location.Z = 0.0;
				SpawnedActor->SetActorLocation(HitResult.Location);
			}
		}
	}
}

void ADynamicShapeController::HandleLeftMouseClick() {
	if (bIsActorMovable) {
		bIsActorMovable = false;
	}
	else {
		bIsActorMovable = true;
	}
}
