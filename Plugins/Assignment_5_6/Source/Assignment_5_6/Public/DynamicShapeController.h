// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionArea.h"
#include "GameFramework/PlayerController.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "MeshGenerationUI.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MeshGenerator.h"
#include "DynamicShapeController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API ADynamicShapeController : public APlayerController {
	
	GENERATED_BODY()

	bool bIsActorMovable;

public:
	ADynamicShapeController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	EShapeType ShapeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Widget")
	TSubclassOf<UMeshGenerationUI> WidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, category="Widget")
	UMeshGenerationUI* Widget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Shape")
	TSubclassOf<ASelectionArea> SelectionAreaClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, category = "Shape")
	ASelectionArea* SelectionArea;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, category = "Shape")
	FVector CurrentShapeBounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Mesh Generator")
	TSubclassOf<AMeshGenerator> MeshGeneratorClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, category = "Mesh Generator")
	AMeshGenerator* MeshGenerator;

	FInputModeGameAndUI InputMode{};

	UFUNCTION()
	void HandleShapeTypeChange(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void GenerateNewSphere(float NewRadius);

	UFUNCTION()
	void GenerateNewBox(float NewValue);

	UFUNCTION()
	void HandleInstanceCountChange(float NewInstanceCount);

	void HandleActorLocationChange();

	void HandleLeftMouseClick();

	UFUNCTION()
	void HandleMeshButtonClick();
	
	UFUNCTION()
	void HandleProgressBar(float ProgressInPercent);
};
