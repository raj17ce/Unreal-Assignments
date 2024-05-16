// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/SelectionWidget.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Actor.h"
#include "InteractiveArchController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
public:
	AInteractiveArchController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	FInputModeGameAndUI InputMode{};

	UPROPERTY()
	AActor* HitResultActor;

	void HandleLeftClick();
	void HandleTabKeyPress();

	void MakeMeshScrollBoxVisible();
	void MakeAllScrollBoxesVisible();

	UFUNCTION()
	void SpawnArchMeshActor(const FMeshData& MeshData);
};