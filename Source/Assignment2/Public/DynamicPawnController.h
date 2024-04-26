// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnAttributeAsset.h"
#include "DynamicPawnController.generated.h"


UENUM()
enum class EPawnType : uint8 {
	ThirdPersonPawn UMETA(DisplayName = "Third Person Pawn"),
	TopDownPawn UMETA(DisplayName = "Top Down Pawn"),
	FirstPersonPawn UMETA(DisplayName = "First Person Pawn")
};

USTRUCT(BlueprintType)
struct FPawnDataTable : public FTableRowBase {

public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	EPawnType PawnType;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	TSubclassOf<class APawn> PawnClass;
};

UCLASS()
class ASSIGNMENT2_API ADynamicPawnController : public APlayerController
{
	GENERATED_BODY()

public:
	ADynamicPawnController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UPawnAttributeAsset* PawnAttributes);

	void SpawnPawn();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* PawnMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* SwapAction;

	AActor* CurrentPawn;
	int32 CurrentPawnIndex;
	EPawnType CurrentPawnType;

	UDataTable* PawnDataTable;
	TArray<FName> PawnNames;
};