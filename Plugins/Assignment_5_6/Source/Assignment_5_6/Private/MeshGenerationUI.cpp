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
	ProgressBarText->SetVisibility(ESlateVisibility::Visible);
	ProgressBarPercent->SetVisibility(ESlateVisibility::Visible);
}

void UMeshGenerationUI::HideProgressBar() {
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	ProgressBarText->SetVisibility(ESlateVisibility::Hidden);
	ProgressBarPercent->SetVisibility(ESlateVisibility::Hidden);
}

void UMeshGenerationUI::EnableMeshButton() {
	GenerateMeshesButton->SetIsEnabled(true);
}

void UMeshGenerationUI::DisableMeshButton() {
	GenerateMeshesButton->SetIsEnabled(false);
}

void UMeshGenerationUI::UpdateProgressBar(float InPercent) {
	ProgressBar->SetPercent(InPercent);

	FText Percent = FText::AsNumber(FMath::RoundToInt(InPercent * 100));
	FText PercentSymbol = FText::FromString(" %");

	ProgressBarPercent->SetText(FText::Format(FText::FromString("{0}{1}"), Percent, PercentSymbol));

	if (InPercent == 1.0f) {
		HideProgressBar();
		EnableMeshButton();
	}
}
