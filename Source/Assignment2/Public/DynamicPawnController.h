// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "DynamicPawnController.generated.h"


UENUM()
enum class EPawnType : uint8 {
	ThirdPersonPawn UMETA(DisplayName = "ThirdPersonPawn"),
	TopDownPawn UMETA(DisplayName = "TopDownPawn")
};

USTRUCT(BlueprintType)
struct FPawnDataTable : public FTableRowBase {

public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	EPawnType PawnType;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	TSubclassOf<class AActor> PawnClass;
};

UCLASS()
class ASSIGNMENT2_API ADynamicPawnController : public APlayerController
{
	GENERATED_BODY()
	
};
