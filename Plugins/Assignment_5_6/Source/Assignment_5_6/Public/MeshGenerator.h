// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshDataAsset.h"
#include "Engine/StaticMesh.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "SelectionArea.h"
#include "FAsyncScatterTask.h"
#include "MeshGenerator.generated.h"

DECLARE_DELEGATE_OneParam(FOnProgressed, float);

UCLASS()
class ASSIGNMENT_5_6_API AMeshGenerator : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Scatter")
	UMeshDataAsset* DataAsset;	

	void InitParams(ASelectionArea* SelectionAreaActor, int32 Instances, EShapeType Shape, FVector DimensionsOfShape);

	void SetNumberOfInstances(int32 Instances);
	int32 GetNumberOfInstances();

	ASelectionArea* GetSelectionArea();

	void SetDimensions(const FVector& NewDimensions);
	FVector GetDimensions();

	void SetShapeType(EShapeType Shape);
	EShapeType GetShapeType();

	FOnProgressed OnProgressed;

	UFUNCTION(BlueprintCallable)
	void ScatterMeshes();

	void AddMeshInstance(const FStaticMeshData& MeshData, const FTransform& Transform);

	void RemoveMeshInstances();

private:
	FAsyncTask<FAsyncScatterTask>* AsyncScatterTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

	UPROPERTY(VisibleDefaultsOnly, Category="Scatter")
	int32 NumberOfInstances;

	int32 GeneratedInstances;

	UPROPERTY(VisibleDefaultsOnly, Category="Scatter")
	ASelectionArea* SelectionArea;

	UPROPERTY(VisibleDefaultsOnly, Category="Scatter")
	EShapeType ShapeType;

	UPROPERTY(VisibleDefaultsOnly, Category="Scatter")
	FVector Dimensions;
};
