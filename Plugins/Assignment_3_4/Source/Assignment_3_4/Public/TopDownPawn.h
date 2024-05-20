// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "InputModifiers.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TopDownPawn.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopDownPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	float OffsetScaleFactor;
	float ArmLengthScaleFactor;

	UInputMappingContext* MappingContext;
	UInputAction* MoveAction;
	UInputAction* ZoomAction;

	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* FloatingPawnMovement;

	void Move(const FInputActionValue& ActionValue);
	void Zoom(const FInputActionValue& ActionValue);
};
