// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

// Sets default values
AMeshGenerator::AMeshGenerator() : NumberOfInstances{100} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMeshGenerator::InitParams(ASelectionArea* SelectionAreaActor, int32 Instances, EShapeType Shape, FVector DimensionsOfShape) {
	SelectionArea = SelectionAreaActor;
	NumberOfInstances = Instances;
	ShapeType = Shape;
	Dimensions = DimensionsOfShape;
}

void AMeshGenerator::SetNumberOfInstances(int32 Instances) {
	NumberOfInstances = Instances;
}

int32 AMeshGenerator::GetNumberOfInstances() {
	return NumberOfInstances;
}

ASelectionArea* AMeshGenerator::GetSelectionArea() {
	return SelectionArea;
}

void AMeshGenerator::SetDimensions(const FVector& NewDimensions) {
	Dimensions = NewDimensions;
}

FVector AMeshGenerator::GetDimensions() {
	return Dimensions;
}

void AMeshGenerator::SetShapeType(EShapeType Shape) {
	ShapeType = Shape;
}

EShapeType AMeshGenerator::GetShapeType() {
	return ShapeType;
}

void AMeshGenerator::ScatterMeshes() {
	
	GeneratedInstances = 0;

	if (AsyncScatterTask && !AsyncScatterTask->IsDone()) {
		AsyncScatterTask->EnsureCompletion();
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}

	AsyncScatterTask = new FAsyncTask<FAsyncScatterTask>(this);
	AsyncScatterTask->StartBackgroundTask();
}

void AMeshGenerator::AddMeshInstance(const FStaticMeshData& MeshData, const FTransform& Transform) {
	
	if(!this && !IsValid(this)) return;

	auto** HISMCPtr = HISMComponents.Find(MeshData.StaticMesh);
	UHierarchicalInstancedStaticMeshComponent* NewHISMC;

	if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel()) {
		NewHISMC = *HISMCPtr;
	}
	else {
		NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
		NewHISMC->SetStaticMesh(MeshData.StaticMesh);
		NewHISMC->RegisterComponentWithWorld(GetWorld());
		HISMComponents.Add(MeshData.StaticMesh, NewHISMC);
		NewHISMC->SetMaterial(0, MeshData.Material);
	}

	AsyncTask(ENamedThreads::GameThread, [NewHISMC, MeshData, Transform, this]() {
		NewHISMC->AddInstance(Transform, false);

		++GeneratedInstances;
		OnProgressed.ExecuteIfBound((float)GeneratedInstances / NumberOfInstances);
	});
}

void AMeshGenerator::RemoveMeshInstances() {
	for (auto& [StaticMesh, HISM] : HISMComponents) {
		if (HISM) {
			HISM->ClearInstances();
		}
	}
}