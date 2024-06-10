// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/SceneComponent.h"
#include "SelectionArea.generated.h"

UENUM()
enum class EShapeType : uint8 {
	Spherical,
	Box
};

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Material")
	UMaterialInterface* ShapeMaterial;

	UFUNCTION(BlueprintCallable)
	void GenerateCube(int32 Section, const FVector& CubeDimensions, float ZOffset);

	UFUNCTION(BlueprintCallable)
	void GenerateSphere(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset);

	void AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3);

	static EShapeType GetShapeTypeFromString(const FString& ShapeType);
};
