// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerationUI.h"

void UMeshGenerationUI::HideBoxDimensions() {
	BoxDimensions->SetVisibility(ESlateVisibility::Hidden);
	SphereRadius->SetVisibility(ESlateVisibility::Visible);
}

void UMeshGenerationUI::HideSphereRadius() {
	BoxDimensions->SetVisibility(ESlateVisibility::Visible);
	SphereRadius->SetVisibility(ESlateVisibility::Hidden);
}

void UMeshGenerationUI::ShowProgressBar() {
	ProgressBar->SetVisibility(ESlateVisibility::Visible);
}

void UMeshGenerationUI::HideProgressBar() {
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UMeshGenerationUI::UpdateProgressBar(float InPercent) {
	ProgressBar->SetPercent(InPercent);

	if (InPercent == 1.0f) {
		HideProgressBar();
	}
}
