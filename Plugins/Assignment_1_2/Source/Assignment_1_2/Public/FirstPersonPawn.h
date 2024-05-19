// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "InputModifiers.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnAttributeAsset.h"
#include "FirstPersonPawn.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API AFirstPersonPawn : public APawn {
	GENERATED_BODY()

public:
	AFirstPersonPawn();

protected:
	virtual void BeginPlay() override;

public:
	UPawnAttributeAsset* PawnAttributeAsset;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* FloatingPawnMovement;

	UInputMappingContext* PawnMappingContext;
	UInputAction* MoveAction;
	UInputAction* LookAction;

	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);
};