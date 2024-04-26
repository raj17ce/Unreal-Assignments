// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnAttributeAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT2_API UPawnAttributeAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPawnAttributeAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight;
};
