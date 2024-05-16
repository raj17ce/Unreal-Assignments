// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectionWidget.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"

void USelectionWidget::NativeConstruct() {
	Super::NativeConstruct();

	MeshScrollBox->MeshSelectionScrollBox->OnMeshThumbnailSelected.BindLambda([this](const FMeshData& MeshData) {
		OnWidgetMeshThumbnailSelected.ExecuteIfBound(MeshData);
	});
}