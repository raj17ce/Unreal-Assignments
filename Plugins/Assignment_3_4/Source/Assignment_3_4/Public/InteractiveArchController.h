// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallSpline.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/SelectionWidget.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Actor.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "PerspectivePawn.h"
#include "OrthographicPawn.h"
#include "IsometricPawn.h"
#include "InteractiveArchController.generated.h"

DECLARE_DELEGATE_OneParam(FMessageDelegate, const FString&)

/**
 * 
 */
UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
public:
	AInteractiveArchController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//Core
	UPROPERTY()
	UInputMappingContext* CurrentMappingContext;

	void SetupPawnSwitchInputComponent();

	int32 CurrentPawnIndex;

	UPROPERTY()
	TArray<TSubclassOf<APawn>> PawnReferences;

	UPROPERTY()
	UInputMappingContext* PawnSwitchMappingContext;

	bool bToggleInputContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* SwitchModeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> SwitchModeWidgetClass;

	UFUNCTION(BlueprintImplementableEvent)
	void BindToggleDispatcher();

	UFUNCTION(BlueprintCallable)
	void ToggleMappingContext();

	void HandlePawnSwitchPKeyPress();

	//Mesh Generator
	void SetupMeshGeneratorInputComponent();

	UPROPERTY()
	UInputMappingContext* MeshGeneratorMappingContext;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY(BlueprintReadWrite)
	USelectionWidget* SelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	FInputModeGameAndUI InputMode{};

	UPROPERTY()
	AActor* HitResultActor;

	void HandleMeshGeneratorLeftClick();
	void HandleMeshGeneratorTabKeyPress();

	void MakeMeshScrollBoxVisible();
	void MakeAllScrollBoxesVisible();

	UFUNCTION()
	void SpawnArchMeshActor(const FMeshData& MeshData);

	UFUNCTION()
	void ChangeActorMaterial(const FMaterialData& MaterialData);

	UFUNCTION()
	void ChangeActorTexture(const FTextureData& TextureData);

	//Wall Generator
	void SetupWallGeneratorInputComponent();

	UPROPERTY()
	UInputMappingContext* WallGeneratorMappingContext;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* NotificationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> NotificationWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HelpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> HelpWidgetWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TArray<AWallSpline*> WallSplines;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TSubclassOf<AWallSpline> SplineType;

	int32 CurrentSplineIndex;

	void HandleWallGeneratorLeftMouseClick();
	void HandleWallGeneratorRightMouseClick();
	void HandleWallGeneratorKeyboardInputJ();
	void HandleWallGeneratorKeyboardInputL();
	void HandleWallGeneratorKeyboardInputX();
	void HandleWallGeneratorKeyboardInputZ();
	void HandleWallGeneratorKeyboardInputDelete();
	void HandleWallGeneratorKeyboardInputEscape();

	FMessageDelegate MessageDelegate;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayMessage(const FString& Message);
};