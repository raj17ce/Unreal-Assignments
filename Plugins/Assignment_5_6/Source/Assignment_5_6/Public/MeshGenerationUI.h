// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "UMG/Public/Components/SpinBox.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "MeshGenerationUI.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API UMeshGenerationUI : public UUserWidget {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ShapeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* SphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* BoxDimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* BoxDimensionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* BoxDimensionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* BoxDimensionZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* InstanceCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* GenerateMeshesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NotificationText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ProgressBarText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ProgressBarPercent;

	void HideBoxDimensions();
	void HideSphereRadius();
	void ShowProgressBar();
	void HideProgressBar();
	void EnableMeshButton();
	void DisableMeshButton();

	void UpdateProgressBar(float InPercent);
};
