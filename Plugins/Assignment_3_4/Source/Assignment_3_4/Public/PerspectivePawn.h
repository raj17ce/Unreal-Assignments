// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "PerspectivePawn.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API APerspectivePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerspectivePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* PerspectivePawnMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveScale;

	void HandleMove(const FInputActionValue& ActionValue);
	void HandleLook(const FInputActionValue& ActionValue);
};
