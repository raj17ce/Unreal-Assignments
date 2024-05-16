// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshSelectionScrollBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "SelectionWidget.generated.h"

DECLARE_DELEGATE_OneParam(FWidgetMeshThumbnailSelected, const FMeshData&)

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* MeshScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* TextureScrollBox;

	virtual void NativeConstruct() override;

	FWidgetMeshThumbnailSelected OnWidgetMeshThumbnailSelected;
};
