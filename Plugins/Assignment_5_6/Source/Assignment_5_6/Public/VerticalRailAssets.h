// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VerticalRailActor.h"
#include "VerticalRailAssets.generated.h"

USTRUCT(Blueprintable)
struct FVerticalRailData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVerticalRailActor> ClassRef;
};

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API UVerticalRailAssets : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VerticalRail")
	TMap<ETopMeshType, FVerticalRailData> DataMap;
};