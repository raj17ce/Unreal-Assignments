// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "UMG/Public/Components/SizeBoxSlot.h"
#include "Types/SlateStructs.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UMG/Public/Components/Widget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	if (InArgs._InMeshAssetManager.IsValid()) {
		MeshAssetManager = InArgs._InMeshAssetManager;
	}

	AssetType = InArgs._InAssetType;

	ScrollBox = SNew(SScrollBox);
	ScrollBox->SetOrientation(Orient_Horizontal);

	ChildSlot
	[
		ScrollBox.ToSharedRef()
	];
	
	switch (AssetType) {
	case EAssetType::Mesh:
		GenerateMeshAssets();
		break;
	case EAssetType::Material:
		GenerateMaterialAssets();
		break;
	case EAssetType::Texture:
		GenerateTextureAssets();
		break;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::GenerateMeshAssets() {
	ScrollBox->ClearChildren();

	if (MeshAssetManager.IsValid() && MeshAssetManager->Meshes.Num() > 0) {
		for (const FMeshData& MeshData : MeshAssetManager->Meshes) {

			PopulateVerticalBox();
			PopulateSizeBox();
			PopulateBorderBox();
		
			SizeBox->SetContent(BorderBox.ToSharedRef());

			CreateMeshThumbnail(MeshData);

			PopulateTextBlock(MeshData.MeshName);

			VerticalBox->AddSlot()[
				SizeBox.ToSharedRef()
			].HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

			VerticalBox->AddSlot()[
				TextBlock.ToSharedRef()
			].AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

			ScrollBox->AddSlot()[
				VerticalBox.ToSharedRef()
			].Padding(5);
		}
	}
}

void SMeshSelectionScrollBox::GenerateMaterialAssets() {
	ScrollBox->ClearChildren();

	if (MeshAssetManager.IsValid() && MeshAssetManager->Materials.Num() > 0) {
		for (const FMaterialData& MaterialData : MeshAssetManager->Materials) {

			PopulateVerticalBox();
			PopulateSizeBox();
			PopulateBorderBox();

			SizeBox->SetContent(BorderBox.ToSharedRef());

			CreateMaterialThumbnail(MaterialData);

			PopulateTextBlock(MaterialData.MaterialName);

			VerticalBox->AddSlot()[
				SizeBox.ToSharedRef()
			].HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

			VerticalBox->AddSlot()[
				TextBlock.ToSharedRef()
			].AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

			ScrollBox->AddSlot()[
				VerticalBox.ToSharedRef()
			].Padding(5);
		}
	}
}

void SMeshSelectionScrollBox::GenerateTextureAssets() {
	ScrollBox->ClearChildren();

	if (MeshAssetManager.IsValid() && MeshAssetManager->Textures.Num() > 0) {
		for (const FTextureData& TextureData : MeshAssetManager->Textures) {

			PopulateVerticalBox();
			PopulateSizeBox();
			PopulateBorderBox();

			SizeBox->SetContent(BorderBox.ToSharedRef());

			CreateTextureThumbnail(TextureData);

			PopulateTextBlock(TextureData.TextureName);

			VerticalBox->AddSlot()[
				SizeBox.ToSharedRef()
			].HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

				VerticalBox->AddSlot()[
					TextBlock.ToSharedRef()
				].AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center).VAlign(EVerticalAlignment::VAlign_Center);

					ScrollBox->AddSlot()[
						VerticalBox.ToSharedRef()
					].Padding(5);
		}
	}
}

void SMeshSelectionScrollBox::PopulateVerticalBox() {
	VerticalBox = SNew(SVerticalBox);
}

void SMeshSelectionScrollBox::PopulateSizeBox() {
	SizeBox = SNew(SBox);

	SizeBox->SetWidthOverride(TAttribute<FOptionalSize>{ 95 });
	SizeBox->SetPadding(FMargin{ 5 });
}

void SMeshSelectionScrollBox::PopulateBorderBox() {
	BorderBox = SNew(SBorder);

	BorderBox->SetHAlign(EHorizontalAlignment::HAlign_Center);
	BorderBox->SetVAlign(EVerticalAlignment::VAlign_Center);

	FSlateBrush* BorderImage = new FSlateBrush();
	BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 5,5,5,5 };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
	OutlineSettings.bUseBrushTransparency = true;

	BorderImage->OutlineSettings = OutlineSettings;

	BorderBox->SetBorderImage(BorderImage);
	BorderBox->SetBorderBackgroundColor(FSlateColor{ FLinearColor{0.181164, 0.40724, 0.423268, 1} });
	BorderBox->SetPadding(FMargin{ 5 });
}

void SMeshSelectionScrollBox::PopulateTextBlock(const FString& MeshName) {
	TextBlock = SNew(STextBlock);

	TextBlock->SetText(FText::FromString(MeshName));
	TextBlock->SetColorAndOpacity(FSlateColor{ FLinearColor{0.015996, 0.021219, 0.030713, 1.0} });

	FTextBlockStyle* TextStyle = new FTextBlockStyle();

	FSlateFontInfo FontStyle;

	UObject* MontserratFontObj = StaticLoadObject(UFont::StaticClass(), NULL, TEXT("/Script/Engine.Font'/Assignment4/Fonts/Montserrat-Bold_Font.Montserrat-Bold_Font'"));

	if (MontserratFontObj) {
		FontStyle.FontObject = MontserratFontObj;
	}
	FontStyle.Size = 14;
	FontStyle.TypefaceFontName = TEXT("Bold");

	TextStyle->Font = FontStyle;

	TextBlock->SetTextStyle(TextStyle);
}

void SMeshSelectionScrollBox::CreateMeshThumbnail(const FMeshData& MeshData) {
	if (auto* Thumbnail = MeshData.MeshIcon) {

		FSlateBrush* ThumbnailBrush = new FSlateBrush();
		ThumbnailBrush->SetResourceObject(Thumbnail);
		ThumbnailBrush->SetImageSize(FVector2D(80, 80));

		TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand)
			.OnMouseButtonDown_Lambda([MeshData, this](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
			if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
				//Todo
				return FReply::Handled();
			}
			return FReply::Unhandled();
				});

		BorderBox->SetContent(ThumbnailImage.ToSharedRef());
	}
}

void SMeshSelectionScrollBox::CreateMaterialThumbnail(const FMaterialData& MaterialData) {
	if (auto* Thumbnail = MaterialData.MaterialIcon) {

		FSlateBrush* ThumbnailBrush = new FSlateBrush();
		ThumbnailBrush->SetResourceObject(Thumbnail);
		ThumbnailBrush->SetImageSize(FVector2D(80, 80));

		TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand)
			.OnMouseButtonDown_Lambda([MaterialData, this](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
			if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
				//Todo
				return FReply::Handled();
			}
			return FReply::Unhandled();
				});

		BorderBox->SetContent(ThumbnailImage.ToSharedRef());
	}
}

void SMeshSelectionScrollBox::CreateTextureThumbnail(const FTextureData& TextureData) {
	if (auto* Thumbnail = TextureData.Texture) {

		FSlateBrush* ThumbnailBrush = new FSlateBrush();
		ThumbnailBrush->SetResourceObject(Thumbnail);
		ThumbnailBrush->SetImageSize(FVector2D(80, 80));

		TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand)
			.OnMouseButtonDown_Lambda([TextureData, this](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
			if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
				//Todo
				return FReply::Handled();
			}
			return FReply::Unhandled();
				});

		BorderBox->SetContent(ThumbnailImage.ToSharedRef());
	}
}
