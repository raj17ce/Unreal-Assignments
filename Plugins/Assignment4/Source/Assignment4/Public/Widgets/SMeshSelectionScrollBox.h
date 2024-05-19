// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MeshAssetManager.h"
#include "Engine/Font.h"

DECLARE_DELEGATE_OneParam(FMeshThumbnailSelected, const FMeshData&)
DECLARE_DELEGATE_OneParam(FMaterialThumbnailSelected, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FTextureThumbnailSelected, const FTextureData&)

UENUM(BlueprintType)
enum class EAssetType : uint8 {
	Mesh,
	Material,
	Texture
};

/**
 * 
 */
class ASSIGNMENT4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAssetManager)
	SLATE_ARGUMENT(EAssetType, InAssetType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> VerticalBox;
	TSharedPtr<SBox> SizeBox;
	TSharedPtr<SBorder> BorderBox;
	TSharedPtr<STextBlock> TextBlock;

	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager;
	EAssetType AssetType;

	void GenerateMeshAssets();
	void GenerateMaterialAssets();
	void GenerateTextureAssets();

	FMeshThumbnailSelected OnMeshThumbnailSelected;
	FMaterialThumbnailSelected OnMaterialThumbnailSelected;
	FTextureThumbnailSelected OnTextureThumbnailSelected;

private:
	void PopulateVerticalBox();
	void PopulateSizeBox();
	void PopulateBorderBox();
	void PopulateTextBlock(const FString& MeshName);

	void CreateMeshThumbnail(const FMeshData& MeshData);
	void CreateMaterialThumbnail(const FMaterialData& MaterialData);
	void CreateTextureThumbnail(const FTextureData& TextureData);
};
