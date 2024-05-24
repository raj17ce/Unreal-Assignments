// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "VerticalRailActor.h"
#include "FenceMeshActor.generated.h"

USTRUCT(Blueprintable)
struct FFenceProperties {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	float Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	float Spacing;
};

UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFenceMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Vertical Rail")
	TSubclassOf<AVerticalRailActor> VerticalRailActorClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Vertical Rail")
	AVerticalRailActor* VerticalRailActor;
};
