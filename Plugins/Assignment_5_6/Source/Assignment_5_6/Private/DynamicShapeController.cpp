// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicShapeController.h"


ADynamicShapeController::ADynamicShapeController() : bIsActorMovable{true}, ShapeType{ EShapeType::Spherical }, Widget{ nullptr }, SelectionArea{nullptr}, MeshGenerator{nullptr} {}

void ADynamicShapeController::BeginPlay() {
	Super::BeginPlay();

	if (WidgetClass) {
		Widget = CreateWidget<UMeshGenerationUI>(this, WidgetClass, "Widget");
		Widget->AddToViewport(1);

		SetShowMouseCursor(true);
		SetInputMode(InputMode);

		Widget->BoxDimensions->SetVisibility(ESlateVisibility::Hidden);

		CurrentShapeBounds = FVector{ Widget->SphereRadius->GetValue() };

		if (SelectionAreaClass) {
			SelectionArea = GetWorld()->SpawnActor<ASelectionArea>(SelectionAreaClass, FVector::ZeroVector, FRotator::ZeroRotator);
		}
		if (MeshGeneratorClass) {
			MeshGenerator = NewObject<AMeshGenerator>(this, MeshGeneratorClass);
			if (SelectionArea) {
				MeshGenerator->InitParams(SelectionArea, FMath::FloorToInt(Widget->InstanceCount->GetValue()), ShapeType, CurrentShapeBounds);
			}
		}

		Widget->HideProgressBar();
		Widget->ShapeType->OnSelectionChanged.AddDynamic(this, &ADynamicShapeController::HandleShapeTypeChange);
		Widget->SphereRadius->OnValueChanged.AddDynamic(this, &ADynamicShapeController::GenerateNewSphere);
		Widget->BoxDimensionX->OnValueChanged.AddDynamic(this, &ADynamicShapeController::GenerateNewBox);
		Widget->BoxDimensionY->OnValueChanged.AddDynamic(this, &ADynamicShapeController::GenerateNewBox);
		Widget->BoxDimensionZ->OnValueChanged.AddDynamic(this, &ADynamicShapeController::GenerateNewBox);
		Widget->InstanceCount->OnValueChanged.AddDynamic(this, &ADynamicShapeController::HandleInstanceCountChange);

		if (MeshGenerator) {
			Widget->GenerateMeshesButton->OnClicked.AddDynamic(this, &ADynamicShapeController::HandleMeshButtonClick);
			
			MeshGenerator->OnProgressed.BindUObject(this, &ADynamicShapeController::HandleProgressBar);
		}

		GenerateNewSphere(CurrentShapeBounds.X);
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

			GenerateNewSphere(CurrentShapeBounds.X);
		}
		else if (ASelectionArea::GetShapeTypeFromString(SelectedItem) == EShapeType::Box) {
			ShapeType = EShapeType::Box;
			Widget->HideSphereRadius();

			GenerateNewBox(CurrentShapeBounds.X);
		}

		if (MeshGenerator) {
			MeshGenerator->SetShapeType(ShapeType);
		}
	}
}

void ADynamicShapeController::GenerateNewSphere(float NewRadius) {
	CurrentShapeBounds = FVector{ NewRadius };
	SelectionArea->GenerateSphere(0, NewRadius, 30, 50, NewRadius);

	if (MeshGenerator) {
		MeshGenerator->SetDimensions(CurrentShapeBounds);
	}
}

void ADynamicShapeController::GenerateNewBox(float NewValue) {
	CurrentShapeBounds = FVector{ Widget->BoxDimensionX->GetValue(), Widget->BoxDimensionY->GetValue(), Widget->BoxDimensionZ->GetValue() };
	SelectionArea->GenerateCube(0, CurrentShapeBounds, CurrentShapeBounds.Z / 2);

	if (MeshGenerator) {
		MeshGenerator->SetDimensions(CurrentShapeBounds);
	}
}

void ADynamicShapeController::HandleInstanceCountChange(float NewInstanceCount) {
	if (MeshGenerator) {
		MeshGenerator->SetNumberOfInstances(FMath::FloorToInt(Widget->InstanceCount->GetValue()));
	}
}

void ADynamicShapeController::HandleActorLocationChange() {
	
	FVector WorldLocation;
	FVector WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(SelectionArea);

		FHitResult HitResult;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams)) {
			if (HitResult.bBlockingHit) {
				HitResult.Location.Z = 0.0;
				SelectionArea->SetActorLocation(HitResult.Location);
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

void ADynamicShapeController::HandleMeshButtonClick() {
	Widget->ShowProgressBar();
	MeshGenerator->ScatterMeshes();
}

void ADynamicShapeController::HandleProgressBar(float ProgressInPercent) {
	Widget->UpdateProgressBar(ProgressInPercent);
}