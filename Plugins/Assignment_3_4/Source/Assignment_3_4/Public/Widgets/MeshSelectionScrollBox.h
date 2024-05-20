// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_3_4_API UMeshSelectionScrollBox : public UWidget {
    GENERATED_BODY()
   public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
    UMeshAssetManager* MeshAssetManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
    EAssetType AssetType;

    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual const FText GetPaletteCategory() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    TSharedPtr<SMeshSelectionScrollBox> MeshSelectionScrollBox;

   private:
    TSharedPtr<SBorder> BorderBox;
    TSharedPtr<SVerticalBox> VerticalBox;
    TSharedPtr<STextBlock> TitleTextBox;
    TSharedPtr<SBox> SizeBox;
    FText AssetsTitle;

    void PopulateAssetsTitle();
    void PopulateBorderBox();
    void PopulateVerticalBox();
    void PopulateTitleTextBox();
    void PopulateSizeBox();
    void PopulateMeshSelectionScrollBox();
};
