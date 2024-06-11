// Fill out your copyright notice in the Description page of Project Settings.

#include "FAsyncScatterTask.h"
#include "MeshGenerator.h"

FAsyncScatterTask::FAsyncScatterTask(AMeshGenerator* NewMeshGenerator) : MeshGenerator{ NewMeshGenerator } {}

void FAsyncScatterTask::DoWork() {
	if (MeshGenerator.IsValid()) {
		if (auto DataAsset = MeshGenerator->DataAsset) {
			TArray<FStaticMeshData> StaticMeshesData = DataAsset->MeshesData;

			for (int i = 0; MeshGenerator.IsValid() && i < MeshGenerator->GetNumberOfInstances(); ++i) {
				
				FVector Origin{0.0};
				if (MeshGenerator.IsValid() && MeshGenerator->GetSelectionArea()->IsValidLowLevel()) {
					Origin = FVector{ MeshGenerator->GetSelectionArea()->GetActorLocation() };
				}
				FVector Position{0.0};

				if (MeshGenerator.IsValid() && MeshGenerator->GetShapeType() == EShapeType::Spherical) {
					Position = FindRandomPointInSphere(Origin, MeshGenerator->GetDimensions());
				}
				else if(MeshGenerator.IsValid()) {
					Position = FindRandomPointInBox(Origin, MeshGenerator->GetDimensions());
				}

				auto MeshDataIndex = FMath::RandRange(0, StaticMeshesData.Num() - 1);
				auto MeshData = StaticMeshesData[MeshDataIndex];

				float Scale = FMath::RandRange(MeshData.MinScale, MeshData.MaxScale);
				float RotationPitch = FMath::FRandRange(MeshData.MinRotation.Pitch, MeshData.MaxRotation.Pitch);
				float RotationYaw = FMath::FRandRange(MeshData.MinRotation.Yaw, MeshData.MaxRotation.Yaw);
				float RotationRoll = FMath::FRandRange(MeshData.MinRotation.Roll, MeshData.MaxRotation.Roll);

				FTransform MeshTransform{ FRotator{ RotationPitch, RotationYaw, RotationRoll }, Position, FVector{Scale} };

				if (MeshGenerator.IsValid()) {
					AsyncTask(ENamedThreads::GameThread, [this, MeshData, MeshTransform]() {
						MeshGenerator->AddMeshInstance(MeshData, MeshTransform);
					});
				}	
				FPlatformProcess::Sleep(0.001f);
			}
		}
	}
}

FVector FAsyncScatterTask::FindRandomPointInSphere(FVector Origin, const FVector& ShapeDimensions) {
	float Theta = FMath::RandRange(0.0f, 2 * PI);
	float Phi = FMath::RandRange(0.0f, PI);

	float UFactor = FMath::RandRange(0.0f, 1.0f);
	float Radius = ShapeDimensions.X * FMath::Pow(UFactor, 1.0f / 3.0f);

	float X = Radius * FMath::Sin(Phi) * FMath::Cos(Theta);
	float Y = Radius * FMath::Sin(Phi) * FMath::Sin(Theta);
	float Z = Radius * FMath::Cos(Phi);

	Origin.Z += ShapeDimensions.Z;
	FVector RandomPoint = Origin + FVector(X, Y, Z);

	return RandomPoint;
}

FVector FAsyncScatterTask::FindRandomPointInBox(FVector Origin, const FVector& ShapeDimensions) {

	Origin.Z += ShapeDimensions.Z / 2;

	FVector BoundingExtent{ ShapeDimensions / 2 };
	FBox BoundingBox{ Origin - BoundingExtent, Origin + BoundingExtent };

	FVector RandomPoint = FMath::RandPointInBox(BoundingBox);

	return RandomPoint;
}