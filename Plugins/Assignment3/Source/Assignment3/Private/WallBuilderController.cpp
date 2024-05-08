// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"
#include "Kismet/KismetSystemLibrary.h"

AWallBuilderController::AWallBuilderController() : CurrentSplineIndex{ 0 } {}

void AWallBuilderController::BeginPlay() {
	Super::BeginPlay();

	MessageDelegate.BindUFunction(this, "DisplayMessage");

	SetShowMouseCursor(true);

	if (SplineType) {
		FName WallSplineName = "Wall Spline" + CurrentSplineIndex;
		AWallSpline* WallSpline = NewObject<AWallSpline>(this, SplineType, WallSplineName);
		WallSplines.Add(WallSpline);

		MessageDelegate.Execute("New WallSpline Actor Created");
	}
}

void AWallBuilderController::SetupInputComponent() {
	
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		auto* MappingContext = NewObject<UInputMappingContext>();

		//Left Mouse Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleLeftMouseClick);

		//Right Mouse Click
		auto* RightMouseClickAction = NewObject<UInputAction>();
		RightMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(RightMouseClickAction, EKeys::RightMouseButton);
		EIC->BindAction(RightMouseClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleRightMouseClick);

		//Keyboard J
		auto* KeyboardJAction = NewObject<UInputAction>();
		KeyboardJAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardJAction, EKeys::J);
		EIC->BindAction(KeyboardJAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputJ);

		//Keyboard L
		auto* KeyboardLAction = NewObject<UInputAction>();
		KeyboardLAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardLAction, EKeys::L);
		EIC->BindAction(KeyboardLAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputL);

		//Keyboard X
		auto* KeyboardXAction = NewObject<UInputAction>();
		KeyboardXAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardXAction, EKeys::X);
		EIC->BindAction(KeyboardXAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputX);

		//Keyboard Z
		auto* KeyboardZAction = NewObject<UInputAction>();
		KeyboardZAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardZAction, EKeys::Z);
		EIC->BindAction(KeyboardZAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputZ);

		//Keyboard Delete
		auto* KeyboardDeleteAction = NewObject<UInputAction>();
		KeyboardDeleteAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardDeleteAction, EKeys::Delete);
		EIC->BindAction(KeyboardDeleteAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputDelete);

		//Keyboard Escape
		auto* KeyboardEscapeAction = NewObject<UInputAction>();
		KeyboardEscapeAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(KeyboardEscapeAction, EKeys::Escape);
		EIC->BindAction(KeyboardEscapeAction, ETriggerEvent::Completed, this, &AWallBuilderController::HandleKeyboardInputEscape);

		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AWallBuilderController::HandleLeftMouseClick() {

	if (CurrentSplineIndex == -1) {
		HandleRightMouseClick();
	}

	FHitResult HitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

	WallSplines[CurrentSplineIndex]->AddNewSplinePoint(HitResult.Location);
	MessageDelegate.Execute("New Spline Point Added");
}

void AWallBuilderController::HandleRightMouseClick() {
	if (SplineType) {
		CurrentSplineIndex = WallSplines.Num();
		FName WallSplineName = "Wall Spline" + CurrentSplineIndex;
		AWallSpline* WallSpline = NewObject<AWallSpline>(this, SplineType, WallSplineName);
		WallSplines.Add(WallSpline);

		MessageDelegate.Execute("New WallSpline Actor Created");
	}
}

void AWallBuilderController::HandleKeyboardInputJ() {
	if (CurrentSplineIndex > 0) {
		--CurrentSplineIndex;
		MessageDelegate.Execute("Previous WallSpline Actor Selected");
	}
	else {
		MessageDelegate.Execute("It's already the first WallSpline Actor");
	}
}

void AWallBuilderController::HandleKeyboardInputL() {
	if (CurrentSplineIndex < WallSplines.Num() - 1) {
		++CurrentSplineIndex;
		MessageDelegate.Execute("Next WallSpline Actor Selected");
	}
	else {
		MessageDelegate.Execute("It's already the last WallSpline Actor");
	}
}

void AWallBuilderController::HandleKeyboardInputX() {
	if (CurrentSplineIndex != -1) {
		WallSplines[CurrentSplineIndex]->Destroy();
		WallSplines[CurrentSplineIndex] = nullptr;
		WallSplines.RemoveAt(CurrentSplineIndex);

		--CurrentSplineIndex;

		MessageDelegate.Execute("Current WallSpline Actor Deleted");

		if (WallSplines.Num() == 0) {
			CurrentSplineIndex = -1;
			return;
		}
		CurrentSplineIndex += WallSplines.Num();
		CurrentSplineIndex = CurrentSplineIndex % WallSplines.Num();
	}
}

void AWallBuilderController::HandleKeyboardInputZ() {

	bool Result{ false };
	if (CurrentSplineIndex != -1) {
		Result = WallSplines[CurrentSplineIndex]->RemoveLastSplinePoint();

		if (!Result) {
			MessageDelegate.Execute("Undo: Creating SplinePoint");
		}
	}

	if (CurrentSplineIndex != -1 && Result) {
		WallSplines[CurrentSplineIndex]->Destroy();
		WallSplines[CurrentSplineIndex] = nullptr;
		WallSplines.RemoveAt(CurrentSplineIndex);
		--CurrentSplineIndex;
		MessageDelegate.Execute("Undo: Creating SplinePoint Actor");

		if (WallSplines.Num() == 0) {
			CurrentSplineIndex = -1;
			return;
		}
		CurrentSplineIndex += WallSplines.Num();
		CurrentSplineIndex = CurrentSplineIndex % WallSplines.Num();
	}
}

void AWallBuilderController::HandleKeyboardInputDelete() {
	for (int32 i = 0; i < WallSplines.Num(); ++i) {
		if (WallSplines[i]) {
			WallSplines[i]->Destroy();
			WallSplines[i] = nullptr;
		}
	}

	if (WallSplines.Num() > 0) {
		WallSplines.Empty();
	}

	CurrentSplineIndex = -1;
	MessageDelegate.Execute("All SplinePoint Actors Deleted");
}

void AWallBuilderController::HandleKeyboardInputEscape() {
	this->ConsoleCommand("quit");
}