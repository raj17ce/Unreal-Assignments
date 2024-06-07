// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASelectionArea::ASelectionArea() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh Component");
	ProceduralMeshComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASelectionArea::CreateSelectionAreaActor(const FVector& Dimensions) {
//	
//
//	if (ShapeType == EShapeType::Box) {
//		GenerateCube(0, Dimensions, Dimensions.Z/2);
//	}
//	else if (ShapeType == EShapeType::Spherical) {
//		GenerateSphere(0, Dimensions.X, FMath::FloorToInt(Dimensions.X / 10), FMath::FloorToInt(Dimensions.X / 5), Dimensions.Z);
//	}
//}
//
//void ASelectionArea::ToggleShape(const FVector& Dimensions) {
//	if (ShapeType == EShapeType::Box) {
//		ShapeType = EShapeType::Spherical;
//	}
//	else if (ShapeType == EShapeType::Spherical) {
//		ShapeType = EShapeType::Box;
//	}
//
//	CreateSelectionAreaActor(Dimensions);
//}

void ASelectionArea::GenerateCube(int32 Section, const FVector& CubeDimensions, float ZOffset) {
	ProceduralMeshComponent->ClearAllMeshSections();

	//Vertices
	TArray<FVector> Vertices;

	//Front
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //0
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //1
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //2
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //3

	//Right
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //4
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //5
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //6
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //7

	//Back
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //8
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //9
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //10
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //11

	//Left
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //12
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //13
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //14
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //15

	//Top
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //16
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //17
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //18
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //19

	//Bottom
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //20
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //21
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //22
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //23

	//Triangles
	TArray<int32> Triangles;

	for (int32 i = 0; i < 24; i += 4) {
		AddTriangle(Triangles, i, i + 1, i + 2);
		AddTriangle(Triangles, i + 2, i + 3, i);
	}

	//Normals
	TArray<FVector> Normals;

	for (int32 i = 0; i < 24; i += 4) {
		FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 1], Vertices[i]));

		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
	}

	//UVs
	TArray<FVector2D> UVs;

	for (int32 i = 0; i < 24; i += 4) {
		UVs.Add(FVector2D{ 0.0,0.0 });
		UVs.Add(FVector2D{ 0.0,1.0 });
		UVs.Add(FVector2D{ 1.0,1.0 });
		UVs.Add(FVector2D{ 1.0,0.0 });
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void ASelectionArea::GenerateSphere(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset) {

	ProceduralMeshComponent->ClearAllMeshSections();

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	//Populating
	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {

		float Theta = PI * RingIndex;
		if (PointsCount > 1) {
			Theta /= (RingCount - 1);
		}

		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		float V = Theta / PI;

		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {

			float Phi = 2 * PI * PointIndex;
			if (PointsCount > 1) {
				Phi /= (PointsCount - 1);
			}
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			float U = Phi / (2 * PI);

			FVector Vertex = FVector{ SinTheta * SinPhi,  SinTheta * CosPhi, CosTheta } *Radius;
			Vertex.Z += ZOffset;

			Vertices.Add(Vertex);
			Normals.Add(UKismetMathLibrary::GetDirectionUnitVector(FVector{ 0,0,ZOffset }, Vertex));
			UVs.Add(FVector2D{ U, V });

			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				AddTriangle(Triangles, CurrentVertexIndex, NextVertexIndex, NextVertexIndex + 1);
				AddTriangle(Triangles, NextVertexIndex + 1, CurrentVertexIndex + 1, CurrentVertexIndex);
			}
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void ASelectionArea::AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3) {
	Triangles.Add(Index1);
	Triangles.Add(Index2);
	Triangles.Add(Index3);
}

EShapeType ASelectionArea::GetShapeTypeFromString(const FString& Shape) {
	EShapeType TempShapeType = EShapeType::Box;
	
	if (Shape == "Spherical") {
		TempShapeType = EShapeType::Spherical;
	}
	else if (Shape == "Box") {
		TempShapeType = EShapeType::Box;
	}

	return TempShapeType;
}