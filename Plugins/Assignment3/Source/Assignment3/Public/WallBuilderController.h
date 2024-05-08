// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallSpline.h"
#include "Engine/World.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "WallBuilderController.generated.h"

DECLARE_DELEGATE_OneParam(FMessageDelegate, const FString&)

/**
 * 
 */
UCLASS()
class ASSIGNMENT3_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWallBuilderController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TArray<AWallSpline*> WallSplines;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TSubclassOf<AWallSpline> SplineType;

	int32 CurrentSplineIndex;

	void HandleLeftMouseClick();
	void HandleRightMouseClick();
	void HandleKeyboardInputJ();
	void HandleKeyboardInputL();
	void HandleKeyboardInputX();
	void HandleKeyboardInputZ();
	void HandleKeyboardInputDelete();
	void HandleKeyboardInputEscape();

	FMessageDelegate MessageDelegate;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayMessage(const FString& Message);
};
