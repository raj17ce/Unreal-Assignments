// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "ArchMeshActor.h"

AInteractiveArchController::AInteractiveArchController() : LastHitLocation{ 0 }, SelectionWidget{nullptr} {}

void AInteractiveArchController::BeginPlay() {
	Super::BeginPlay();

	SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);

	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);

	SelectionWidget->OnWidgetMeshThumbnailSelected.BindUFunction(this, "SpawnArchMeshActor");
	SelectionWidget->OnWidgetMaterialThumbnailSelected.BindUFunction(this, "ChangeActorMaterial");
	SelectionWidget->OnWidgetTextureThumbnailSelected.BindUFunction(this, "ChangeActorTexture");
}

void AInteractiveArchController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		auto* MappingContext = NewObject<UInputMappingContext>();

		//Left-Click Mapping
		auto* LeftClickAction = NewObject<UInputAction>();
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleLeftClick);

		//Tab-Key Mapping

		auto* TabKeyPressAction = NewObject<UInputAction>();
		TabKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(TabKeyPressAction, EKeys::Tab);

		EIC->BindAction(TabKeyPressAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleTabKeyPress);

		if (auto* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsytem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AInteractiveArchController::HandleLeftClick() {
	if (!SelectionWidget->IsInViewport()) {
		SelectionWidget->AddToViewport();
		SelectionWidget->SetVisibility(ESlateVisibility::Visible);

		SetInputMode(InputMode);
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

void AInteractiveArchController::HandleTabKeyPress() {
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