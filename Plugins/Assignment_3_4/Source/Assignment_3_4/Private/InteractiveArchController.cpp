// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "ArchMeshActor.h"

AInteractiveArchController::AInteractiveArchController() : CurrentMappingContext{nullptr}, CurrentPawnIndex{ -1 }, bToggleInputContext{ true }, LastHitLocation{ 0 }, SelectionWidget{ nullptr }, CurrentSplineIndex{ 0 } {
	PawnReferences.Add(APerspectivePawn::StaticClass());
	PawnReferences.Add(AOrthographicPawn::StaticClass());
	PawnReferences.Add(AIsometricPawn::StaticClass());
}

void AInteractiveArchController::BeginPlay() {
	Super::BeginPlay();

	//Selection Widget
	SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);

	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);

	SelectionWidget->OnWidgetMeshThumbnailSelected.BindUFunction(this, "SpawnArchMeshActor");
	SelectionWidget->OnWidgetMaterialThumbnailSelected.BindUFunction(this, "ChangeActorMaterial");
	SelectionWidget->OnWidgetTextureThumbnailSelected.BindUFunction(this, "ChangeActorTexture");

	//Notification Widget
	NotificationWidget = CreateWidget<UUserWidget>(this, NotificationWidgetClass);

	//Help Widget
	HelpWidget = CreateWidget<UUserWidget>(this, HelpWidgetWidgetClass);

	//Switch Mode Widget
	SwitchModeWidget = CreateWidget<UUserWidget>(this, SwitchModeWidgetClass);

	SwitchModeWidget->AddToViewport(5);

	BindToggleDispatcher();

	MessageDelegate.BindUFunction(this, "DisplayMessage");

	SetShowMouseCursor(true);

	if (SplineType) {
		FName WallSplineName = "Wall Spline" + CurrentSplineIndex;
		AWallSpline* WallSpline = NewObject<AWallSpline>(this, SplineType, WallSplineName);
		WallSplines.Add(WallSpline);

		MessageDelegate.Execute("New WallSpline Actor Created");
	}

	SetInputMode(InputModeGameAndUI);

	HandlePawnSwitchPKeyPress();
}

void AInteractiveArchController::SetupInputComponent() {
	Super::SetupInputComponent();

	SetupPawnSwitchInputComponent();
	SetupMeshGeneratorInputComponent();
	SetupWallGeneratorInputComponent();

	if (auto* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		if (PawnSwitchMappingContext) {
			Subsytem->AddMappingContext(PawnSwitchMappingContext, 0);
		}
		if (MeshGeneratorMappingContext) {
			Subsytem->AddMappingContext(MeshGeneratorMappingContext, 0);
			CurrentMappingContext = MeshGeneratorMappingContext;
		}
	}
}

void AInteractiveArchController::SetupPawnSwitchInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		PawnSwitchMappingContext = NewObject<UInputMappingContext>();

		//P-Key Mapping

		auto* PKeyPressAction = NewObject<UInputAction>();
		PKeyPressAction->ValueType = EInputActionValueType::Boolean;

		PawnSwitchMappingContext->MapKey(PKeyPressAction, EKeys::P);

		EIC->BindAction(PKeyPressAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandlePawnSwitchPKeyPress);
	}
}

void AInteractiveArchController::HandlePawnSwitchPKeyPress() {
	CurrentPawnIndex = (CurrentPawnIndex + 1) % PawnReferences.Num();

	if (GetPawn()) {
		GetPawn()->Destroy();
	}

	if (auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
		SubSystem->ClearAllMappings();
	}

	FActorSpawnParameters SpawnActorParams;
	SpawnActorParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rotator = FRotator::ZeroRotator;

	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnReferences[CurrentPawnIndex], LastHitLocation + FVector{ 0,0, 50 }, Rotator, SpawnActorParams);

	if (SpawnedPawn) {
		Possess(SpawnedPawn);
	}

	if (auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
		if (PawnSwitchMappingContext) {
			SubSystem->AddMappingContext(PawnSwitchMappingContext, 0);
		}
		if (CurrentMappingContext) {
			SubSystem->AddMappingContext(CurrentMappingContext, 0);
		}
	}
}

void AInteractiveArchController::SetupMeshGeneratorInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		MeshGeneratorMappingContext = NewObject<UInputMappingContext>();

		//Left-Click Mapping
		auto* LeftClickAction = NewObject<UInputAction>();
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		MeshGeneratorMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleMeshGeneratorLeftClick);

		//Tab-Key Mapping

		auto* TabKeyPressAction = NewObject<UInputAction>();
		TabKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MeshGeneratorMappingContext->MapKey(TabKeyPressAction, EKeys::Tab);

		EIC->BindAction(TabKeyPressAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleMeshGeneratorTabKeyPress);
	}
}

void AInteractiveArchController::HandleMeshGeneratorLeftClick() {
	if (!SelectionWidget->IsInViewport()) {
		SelectionWidget->AddToViewport();
		SelectionWidget->SetVisibility(ESlateVisibility::Visible);

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}

	double MouseX{}, MouseY{};

	if (GetMousePosition(MouseX, MouseY)) {

		FVector WorldLocation{}, WorldDirection{};

		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {

			FVector EndLocation = WorldLocation + WorldDirection * 10000;

			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(GetPawn());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndLocation, ECollisionChannel::ECC_Visibility, QueryParams)) {
				
				if (HitResult.GetActor()) {
					HitResultActor = HitResult.GetActor();
				}
				
				if (HitResultActor->IsA<AArchMeshActor>()) {
					LastHitLocation = HitResult.Location;

					GetPawn()->SetActorLocation(LastHitLocation + FVector{ 0, 0, 100 });

					MakeAllScrollBoxesVisible();
				}
				else {
					LastHitLocation = HitResult.Location;

					MakeMeshScrollBoxVisible();
				}
			}
		}
	}
}

void AInteractiveArchController::HandleMeshGeneratorTabKeyPress() {
	if (SelectionWidget->IsInViewport()) {
		if (SelectionWidget->GetVisibility() == ESlateVisibility::Hidden) {
			SelectionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else if (SelectionWidget->GetVisibility() == ESlateVisibility::Visible) {
			SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AInteractiveArchController::MakeMeshScrollBoxVisible() {
	SelectionWidget->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
}

void AInteractiveArchController::MakeAllScrollBoxesVisible() {
	SelectionWidget->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Visible);
}

void AInteractiveArchController::SpawnArchMeshActor(const FMeshData& MeshData) {
	if (MeshData.Mesh) {

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		UMaterialInstanceDynamic* DynamicMaterial{};

		if (HitResultActor->IsA<AArchMeshActor>()) {
			LastHitLocation = HitResultActor->GetActorLocation();
			
			auto* CurrentActor = Cast<AArchMeshActor>(HitResultActor);

			FBox HitMeshBoundingBox = CurrentActor->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox();
			LastHitLocation.Z += HitMeshBoundingBox.Min.Z;

			DynamicMaterial = Cast<UMaterialInstanceDynamic>(CurrentActor->GetStaticMeshComponent()->GetMaterial(0));
			if (!DynamicMaterial) {
				DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentActor->GetStaticMeshComponent()->GetMaterial(0), NULL);
			}

			HitResultActor->Destroy();
			HitResultActor = nullptr;
		}

		FBox MeshBoundingBox = MeshData.Mesh->GetBoundingBox();
		LastHitLocation.Z += (-1 * MeshBoundingBox.Min.Z);

		if (AArchMeshActor* SpawnedActor = GetWorld()->SpawnActor<AArchMeshActor>(LastHitLocation, FRotator::ZeroRotator, ActorSpawnParameters)) {
			SpawnedActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);

			HitResultActor = SpawnedActor;

			if (DynamicMaterial) {
				SpawnedActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
			}

			MakeAllScrollBoxesVisible();
		}

		GetPawn()->SetActorLocation(LastHitLocation + FVector{ 0, 0, 100 });
	}
}

void AInteractiveArchController::ChangeActorMaterial(const FMaterialData& MaterialData) {
	if (MaterialData.Material) {
		if (HitResultActor->IsA<AArchMeshActor>()) {
			auto* CurrentActor = Cast<AArchMeshActor>(HitResultActor);

			CurrentActor->GetStaticMeshComponent()->SetMaterial(0, MaterialData.Material);
		}
	}
}

void AInteractiveArchController::ChangeActorTexture(const FTextureData& TextureData) {
	if (TextureData.Texture && HitResultActor->IsA<AArchMeshActor>()) {
		auto* CurrentActor = Cast<AArchMeshActor>(HitResultActor);

		if (auto* Material = CurrentActor->GetStaticMeshComponent()->GetMaterial(0)) {
			
			auto* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

			if (!DynamicMaterial) {
				DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
			}

			if (DynamicMaterial) {
				DynamicMaterial->SetTextureParameterValue("SourceTexture", TextureData.Texture);
				CurrentActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
			}
		}
	}
}

void AInteractiveArchController::SetupWallGeneratorInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		WallGeneratorMappingContext = NewObject<UInputMappingContext>();

		//Left Mouse Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorLeftMouseClick);

		//Right Mouse Click
		auto* RightMouseClickAction = NewObject<UInputAction>();
		RightMouseClickAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(RightMouseClickAction, EKeys::RightMouseButton);
		EIC->BindAction(RightMouseClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorRightMouseClick);

		//Keyboard J
		auto* KeyboardJAction = NewObject<UInputAction>();
		KeyboardJAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardJAction, EKeys::J);
		EIC->BindAction(KeyboardJAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputJ);

		//Keyboard L
		auto* KeyboardLAction = NewObject<UInputAction>();
		KeyboardLAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardLAction, EKeys::L);
		EIC->BindAction(KeyboardLAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputL);

		//Keyboard X
		auto* KeyboardXAction = NewObject<UInputAction>();
		KeyboardXAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardXAction, EKeys::X);
		EIC->BindAction(KeyboardXAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputX);

		//Keyboard Z
		auto* KeyboardZAction = NewObject<UInputAction>();
		KeyboardZAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardZAction, EKeys::Z);
		EIC->BindAction(KeyboardZAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputZ);

		//Keyboard Delete
		auto* KeyboardDeleteAction = NewObject<UInputAction>();
		KeyboardDeleteAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardDeleteAction, EKeys::Delete);
		EIC->BindAction(KeyboardDeleteAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputDelete);

		//Keyboard Escape
		auto* KeyboardEscapeAction = NewObject<UInputAction>();
		KeyboardEscapeAction->ValueType = EInputActionValueType::Boolean;

		WallGeneratorMappingContext->MapKey(KeyboardEscapeAction, EKeys::Escape);
		EIC->BindAction(KeyboardEscapeAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleWallGeneratorKeyboardInputEscape);
	}
}

void AInteractiveArchController::HandleWallGeneratorLeftMouseClick() {

	if (CurrentSplineIndex == -1) {
		HandleWallGeneratorRightMouseClick();
	}

	FHitResult HitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

	WallSplines[CurrentSplineIndex]->AddNewSplinePoint(HitResult.Location);
	MessageDelegate.Execute("New Spline Point Added");
}

void AInteractiveArchController::HandleWallGeneratorRightMouseClick() {
	if (SplineType) {
		CurrentSplineIndex = WallSplines.Num();
		FName WallSplineName = "Wall Spline" + CurrentSplineIndex;
		AWallSpline* WallSpline = NewObject<AWallSpline>(this, SplineType, WallSplineName);
		WallSplines.Add(WallSpline);

		MessageDelegate.Execute("New WallSpline Actor Created");
	}
}

void AInteractiveArchController::HandleWallGeneratorKeyboardInputJ() {
	if (CurrentSplineIndex > 0) {
		--CurrentSplineIndex;
		MessageDelegate.Execute("Previous WallSpline Actor Selected");
	}
	else {
		MessageDelegate.Execute("It's already the first WallSpline Actor");
	}
}

void AInteractiveArchController::HandleWallGeneratorKeyboardInputL() {
	if (CurrentSplineIndex < WallSplines.Num() - 1) {
		++CurrentSplineIndex;
		MessageDelegate.Execute("Next WallSpline Actor Selected");
	}
	else {
		MessageDelegate.Execute("It's already the last WallSpline Actor");
	}
}

void AInteractiveArchController::HandleWallGeneratorKeyboardInputX() {
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

void AInteractiveArchController::HandleWallGeneratorKeyboardInputZ() {

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

void AInteractiveArchController::HandleWallGeneratorKeyboardInputDelete() {
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

void AInteractiveArchController::HandleWallGeneratorKeyboardInputEscape() {
	this->ConsoleCommand("quit");
}

void AInteractiveArchController::ToggleMappingContext() {
	if (auto* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {

		if (bToggleInputContext) {
			Subsytem->RemoveMappingContext(MeshGeneratorMappingContext);
			SelectionWidget->RemoveFromParent();

			if (WallGeneratorMappingContext) {
				Subsytem->AddMappingContext(WallGeneratorMappingContext, 0);
				CurrentMappingContext = WallGeneratorMappingContext;

				if (NotificationWidget && HelpWidget) {
					NotificationWidget->AddToViewport();
					HelpWidget->AddToViewport();
				}
			}

			bToggleInputContext = !bToggleInputContext;
		}
		else {
			Subsytem->RemoveMappingContext(WallGeneratorMappingContext);
			NotificationWidget->RemoveFromParent();
			HelpWidget->RemoveFromParent();

			if (MeshGeneratorMappingContext) {
				Subsytem->AddMappingContext(MeshGeneratorMappingContext, 0);
				CurrentMappingContext = MeshGeneratorMappingContext;
			}

			bToggleInputContext = !bToggleInputContext;
		}
	}
}
