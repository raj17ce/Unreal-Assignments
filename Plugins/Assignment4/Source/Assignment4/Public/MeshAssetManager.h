// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

USTRUCT(Blueprintable)
struct FMeshData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	FString MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UTexture2D* MeshIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMesh* Mesh;
};

USTRUCT(Blueprintable)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FString MaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	float Roughness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FLinearColor BaseColor;
};

USTRUCT(Blueprintable)
struct FTextureData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Texture")
	FString TextureName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Texture")
	UTexture2D* Texture;
};

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TArray<FMeshData> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	TArray<FMaterialData> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Texture")
	TArray<FTextureData> Textures;
};
