// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget() {

	//Populating all the objects

	PopulateBorderBox();
	PopulateVerticalBox();
	PopulateTitleTextBox();
	PopulateSizeBox();
	PopulateMeshSelectionScrollBox();

	//Adding Slots and Content

	BorderBox->SetContent(VerticalBox.ToSharedRef());

	VerticalBox->AddSlot()[
		TitleTextBox.ToSharedRef()
	].Padding(0,5,0,0).HAlign(HAlign_Center).VAlign(VAlign_Center).AutoHeight();

	VerticalBox->AddSlot()[
		SizeBox.ToSharedRef()
	].Padding(5).HAlign(HAlign_Center).VAlign(VAlign_Bottom);

	SizeBox->SetContent(MeshSelectionScrollBox.ToSharedRef());

	return BorderBox.ToSharedRef();
}

void UMeshSelectionScrollBox::PopulateAssetsTitle() {
	switch (AssetType) {
	case EAssetType::Mesh:
		AssetsTitle = FText::FromString("Mesh Assets");
		break;
	case EAssetType::Material:
		AssetsTitle = FText::FromString("Material Assets");
		break;
	case EAssetType::Texture:
		AssetsTitle = FText::FromString("Texture Assets");
		break;
	}
}

void UMeshSelectionScrollBox::PopulateBorderBox() {
	BorderBox = SNew(SBorder);

	BorderBox->SetHAlign(EHorizontalAlignment::HAlign_Center);
	BorderBox->SetVAlign(EVerticalAlignment::VAlign_Center);
	BorderBox->SetPadding(FMargin(5));

	FSlateBrush* BorderImage = new FSlateBrush();
	BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 8,8,8,8 };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
	OutlineSettings.bUseBrushTransparency = true;

	BorderImage->OutlineSettings = OutlineSettings;

	BorderBox->SetBorderImage(BorderImage);
	BorderBox->SetBorderBackgroundColor(FSlateColor{ FLinearColor{0.854993, 0.854993, 0.854993, 0.8} });
}

void UMeshSelectionScrollBox::PopulateVerticalBox() {
	VerticalBox = SNew(SVerticalBox);
}

void UMeshSelectionScrollBox::PopulateTitleTextBox() {
	TitleTextBox = SNew(STextBlock);

	PopulateAssetsTitle();
	TitleTextBox->SetText(AssetsTitle);
	TitleTextBox->SetColorAndOpacity(FSlateColor{ FLinearColor{0.015996, 0.021219, 0.030713, 1.0} });

	FTextBlockStyle* TextStyle = new FTextBlockStyle();

	FSlateFontInfo FontStyle;

	UObject* MontserratFontObj = StaticLoadObject(UFont::StaticClass(), NULL, TEXT("/Script/Engine.Font'/Assignment4/Fonts/Montserrat-Bold_Font.Montserrat-Bold_Font'"));

	if (MontserratFontObj) {
		FontStyle.FontObject = MontserratFontObj;
	}
	FontStyle.Size = 18;
	FontStyle.TypefaceFontName = TEXT("Bold");

	TextStyle->Font = FontStyle;

	TitleTextBox->SetTextStyle(TextStyle);
}

void UMeshSelectionScrollBox::PopulateSizeBox() {
	SizeBox = SNew(SBox);

	SizeBox->SetWidthOverride(480);
	SizeBox->SetHeightOverride(140);

	SizeBox->SetHAlign(HAlign_Fill);
	SizeBox->SetVAlign(VAlign_Fill);
}

void UMeshSelectionScrollBox::PopulateMeshSelectionScrollBox() {
	MeshSelectionScrollBox = SNew(SMeshSelectionScrollBox).InMeshAssetManager(MeshAssetManager).InAssetType(AssetType);
}

const FText UMeshSelectionScrollBox::GetPaletteCategory() {
	return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	BorderBox.Reset();
	VerticalBox.Reset();
	TitleTextBox.Reset();
	SizeBox.Reset();
	MeshSelectionScrollBox.Reset();
}