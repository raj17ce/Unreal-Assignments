// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh Component");
	ProceduralMeshComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AVerticalRailActor::CreateVerticalRailActor(const FVector& RailingDimensions) {

	ProceduralMeshComponent->ClearAllMeshSections();

	GenerateCube(0, RailingDimensions, 0.0f);
	GenerateTopMesh(RailingDimensions);
}

void AVerticalRailActor::GenerateTopMesh(const FVector& RailingDimensions) {
	switch (TopMeshType) {
	case ETopMeshType::WindsorTurnedCapital:
		GenerateCube(1, FVector{ RailingDimensions.X, RailingDimensions.Y, RailingDimensions.X / 4 }, (RailingDimensions.Z/2) + (RailingDimensions.X / 8));
		GenerateBellShape(2, RailingDimensions.X / 2, RailingDimensions.X / 4, RailingDimensions.X / 4, 1.0f, 50, 15, (RailingDimensions.Z / 2) + (RailingDimensions.X / 2));
		GenerateSphere(3, RailingDimensions.X / 2, 15, 75, (RailingDimensions.Z / 2) + (RailingDimensions.X));
		GenerateCone(4, RailingDimensions.X/3, (2*RailingDimensions.X)/3, 10, 50, (RailingDimensions.Z / 2) + ((5 * RailingDimensions.X )/ 4));
		break;
	case ETopMeshType::RoundTurnedCapital:
		GenerateCube(1, FVector{RailingDimensions.X, RailingDimensions.Y, RailingDimensions.X/4}, (RailingDimensions.Z / 2) + (RailingDimensions.X / 8));
		GenerateBellShape(2, RailingDimensions.X/2, RailingDimensions.X/4, RailingDimensions.X/4, 1.0f, 50, 15, (RailingDimensions.Z / 2) + (RailingDimensions.X / 2));
		GenerateSphere(3, RailingDimensions.X / 2, 15, 75, (RailingDimensions.Z / 2) + (RailingDimensions.X));
		break;
	case ETopMeshType::AcornCapital:
		GenerateCube(1, FVector{ RailingDimensions.X, RailingDimensions.Y, RailingDimensions.X / 4 }, (RailingDimensions.Z / 2) + (RailingDimensions.X / 8));
		GenerateBellShape(2, RailingDimensions.X / 2, RailingDimensions.X / 4, RailingDimensions.X / 4, 1.0f, 50, 15, (RailingDimensions.Z / 2) + (RailingDimensions.X / 2));
		GenerateOval(3, RailingDimensions.X / 2, 15, 75, (RailingDimensions.Z / 2) + (1.3 * RailingDimensions.X), 1.5);
		break;
	case ETopMeshType::GothicStarTop:
		//To-Do
		break;
	case ETopMeshType::RoundedOverTop:
		GenerateCube(1, FVector{ (2 * RailingDimensions.X) / 3, (2 * RailingDimensions.Y) / 3, RailingDimensions.X / 10 }, (RailingDimensions.Z / 2) + (RailingDimensions.X / 20));
		GenerateCube(2, FVector{ RailingDimensions.X, RailingDimensions.Y, (2 * RailingDimensions.X) / 3 }, (RailingDimensions.Z / 2) + ((13 * RailingDimensions.X) / 30));
		GenerateHorizontalHalfCylinder(3, RailingDimensions.X/2, RailingDimensions.Y, 10, 50, FVector{0, - (RailingDimensions.Y/2) , (RailingDimensions.Z / 2) + ((23 * RailingDimensions.X) / 30) });
		break;
	case ETopMeshType::RoundedStarTop:
		//To-Do
		break;
	case ETopMeshType::PyramidTop:
		GenerateCube(1, FVector{ (2*RailingDimensions.X)/3, (2 * RailingDimensions.Y) / 3, RailingDimensions.X / 10 }, (RailingDimensions.Z / 2) + (RailingDimensions.X / 20));
		GenerateCube(2, FVector{ RailingDimensions.X, RailingDimensions.Y, (2*RailingDimensions.X) / 3 }, (RailingDimensions.Z / 2) + ((13*RailingDimensions.X) / 30));
		GeneratePyramid(3, FVector{RailingDimensions.X, RailingDimensions.Y, (2 * RailingDimensions.X) / 3 }, (RailingDimensions.Z / 2) + ((11*RailingDimensions.X) / 10));
		break;
	}
}

void AVerticalRailActor::GenerateCube(int32 Section, const FVector& CubeDimensions, float ZOffset) {
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

void AVerticalRailActor::GenerateSphere(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset) {

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

			float U = Phi / (2*PI);

			FVector Vertex = FVector{ SinTheta * SinPhi,  SinTheta * CosPhi, CosTheta } *Radius;
			Vertex.Z += ZOffset;

			Vertices.Add(Vertex);
			Normals.Add(UKismetMathLibrary::GetDirectionUnitVector(FVector{ 0,0,ZOffset }, Vertex));
			UVs.Add(FVector2D{ U, V});

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

void AVerticalRailActor::GenerateOval(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset, float ZFactor) {

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

			FVector Vertex = FVector{ SinTheta * SinPhi,  SinTheta * CosPhi, CosTheta * ZFactor } *Radius;
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

void AVerticalRailActor::GenerateBellShape(int32 Section, float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 SlicesCount, int StacksCount, float ZOffset) {

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	//Vertices
	for (int32 StackIndex = 0; StackIndex < StacksCount; ++StackIndex) {
		float Theta = StackIndex * PI / (StacksCount - 1);

		// Bell profile function for radius interpolation with curvature
		float t = (Theta * 2.0f) / PI;
		float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor));

		for (int32 SliceIndex = 0; SliceIndex < SlicesCount; ++SliceIndex) {

			float Phi = (SliceIndex * 2.0f * PI) / (SlicesCount - 1);
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			FVector Vertex = FVector{ CurrentRadius * SinPhi,  CurrentRadius * CosPhi, Height * (t - 1.0f) };
			Vertex.Z += ZOffset;

			Vertices.Add(Vertex);

			// UV mapping
			UVs.Add(FVector2D((float)SliceIndex / (SlicesCount - 1), (float)StackIndex / (StacksCount - 1)));
		}
	}

	//Triangles
	for (int32 i = 0; i < StacksCount - 1; i++) {
		for (int32 j = 0; j < SlicesCount - 1; j++) {
			int32 BottomLeft = i * SlicesCount + j;
			int32 BottomRight = BottomLeft + 1;
			int32 TopLeft = BottomLeft + SlicesCount;
			int32 TopRight = TopLeft + 1;

			// First triangle
			AddTriangle(Triangles, TopLeft, BottomLeft, BottomRight);
			//AddTriangle(Triangles, TopLeft, BottomRight, BottomLeft);

			// Second triangle
			AddTriangle(Triangles, BottomRight, TopRight, TopLeft);
			//AddTriangle(Triangles, BottomRight, TopLeft, TopRight);
		}
	}

	for (int32 i = 0; i < StacksCount - 1; i++) {
		for (int32 j = 0; j < SlicesCount - 1; j++) {
			int32 BottomLeft = i * SlicesCount + j;
			int32 BottomRight = BottomLeft + 1;
			int32 TopLeft = BottomLeft + SlicesCount;
			int32 TopRight = TopLeft + 1;

			auto Normal1 = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[BottomLeft], Vertices[BottomRight]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[BottomLeft], Vertices[TopLeft]));
			auto Normal2 = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[TopRight], Vertices[TopLeft]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[TopRight], Vertices[BottomRight]));

			Normals.Add(Normal1);
			Normals.Add(Normal2);
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void AVerticalRailActor::GenerateCone(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, float ZOffset) {

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	//Populating
	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {

			float Phi = 2 * PI * PointIndex;
			if (PointsCount > 1) {
				Phi /= (PointsCount - 1);
			}
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			float DynamicRadius = Radius * ((float)RingIndex / (RingCount - 1));

			FVector Vertex = FVector{ DynamicRadius * SinPhi,  DynamicRadius * CosPhi, Length * (1 - ((float)RingIndex / (RingCount - 1))) };
			Vertex.Z += ZOffset;

			Vertices.Add(Vertex);
			UVs.Add(FVector2D{ Phi / (2 * PI),  (1.0 - ((float)RingIndex / (RingCount - 1))) });

			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				AddTriangle(Triangles, CurrentVertexIndex, NextVertexIndex, NextVertexIndex + 1);
				AddTriangle(Triangles, NextVertexIndex + 1, CurrentVertexIndex + 1, CurrentVertexIndex);
			}
		}
	}
	
	//Normals
	for (int32 RingIndex = 0; RingIndex < RingCount-1; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount-1; ++PointIndex) {
			int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
			int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

			FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
			Normals.Add(Normal);

			if (PointIndex == PointsCount - 2) {
				Normals.Add(Normal);
			}
		}
	}

	for (int32 PointIndex = 0; PointIndex < PointsCount - 1; ++PointIndex) {
		int32 CurrentVertexIndex = (RingCount-2) * PointsCount + PointIndex;
		int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

		FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
		Normals.Add(Normal);

		if (PointIndex == PointsCount - 2) {
			Normals.Add(Normal);
		}
	}

	//Base Circle
	Vertices.Add(FVector(0.0f, 0.0f, ZOffset));
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
		float Phi = 2 * PI * PointIndex;
		if (PointsCount > 1) {
			Phi /= (PointsCount - 1);
		}
		float X = Radius * FMath::Sin(Phi);
		float Y = Radius * FMath::Cos(Phi);
		float Z = ZOffset;

		Vertices.Add(FVector(X, Y, Z));
		UVs.Add(FVector2D(Y / Radius / 2.0f + 0.5f, X / Radius / 2.0f + 0.5f));
	}

	FVector Normal = FVector::CrossProduct(Vertices[RingCount * PointsCount + 1], Vertices[RingCount * PointsCount + 2]).GetSafeNormal();
	Normals.Add(Normal);

	//Triangles and Normals
	for (int32 i = 1; i <= PointsCount; ++i) {
		if (i < PointsCount) {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + i + 1));
		}
		else {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + 1));
		}
		Normals.Add(Normal);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void AVerticalRailActor::GenerateCylinder(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, float ZOffset) {

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	//Populating
	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {

			float Phi = 2 * PI * PointIndex;
			if (PointsCount > 1) {
				Phi /= (PointsCount - 1);
			}
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			FVector Vertex = FVector{ Radius * SinPhi,  Radius * CosPhi, Length * (1 - ((float)RingIndex / (RingCount - 1))) };
			Vertex.Z += ZOffset;

			Vertices.Add(Vertex);
			UVs.Add(FVector2D{ Phi / (2 * PI),  (1 - ((float)RingIndex / (RingCount - 1))) });

			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				AddTriangle(Triangles, CurrentVertexIndex, NextVertexIndex, NextVertexIndex + 1);
				AddTriangle(Triangles, NextVertexIndex + 1, CurrentVertexIndex + 1, CurrentVertexIndex);
			}
		}
	}

	//Normals
	for (int32 RingIndex = 0; RingIndex < RingCount - 1; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount - 1; ++PointIndex) {
			int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
			int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

			FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
			Normals.Add(Normal);

			if (PointIndex == PointsCount - 2) {
				Normals.Add(Normal);
			}
		}
	}

	for (int32 PointIndex = 0; PointIndex < PointsCount - 1; ++PointIndex) {
		int32 CurrentVertexIndex = (RingCount - 2) * PointsCount + PointIndex;
		int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

		FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
		Normals.Add(Normal);

		if (PointIndex == PointsCount - 2) {
			Normals.Add(Normal);
		}
	}

	//Base Circle
	Vertices.Add(FVector(0.0f, 0.0f, ZOffset));
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
		float Phi = 2 * PI * PointIndex;
		if (PointsCount > 1) {
			Phi /= (PointsCount - 1);
		}
		float X = Radius * FMath::Sin(Phi);
		float Y = Radius * FMath::Cos(Phi);
		float Z = ZOffset;

		Vertices.Add(FVector(X, Y, Z));
		UVs.Add(FVector2D(Y / Radius / 2.0f + 0.5f, X / Radius / 2.0f + 0.5f));
	}

	FVector Normal = FVector::CrossProduct(Vertices[RingCount * PointsCount + 1], Vertices[RingCount * PointsCount + 2]).GetSafeNormal();
	Normals.Add(Normal);

	//Triangles and Normals
	for (int32 i = 1; i <= PointsCount; ++i) {
		if (i < PointsCount) {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + i + 1));
		}
		else {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + 1));
		}
		Normals.Add(Normal);
	}

	//Top Circle
	Vertices.Add(FVector(0.0f, 0.0f, ZOffset + Length));
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
		float Phi = 2 * PI * PointIndex;
		if (PointsCount > 1) {
			Phi /= (PointsCount - 1);
		}
		float X = Radius * FMath::Sin(Phi);
		float Y = Radius * FMath::Cos(Phi);
		float Z = ZOffset + Length;

		Vertices.Add(FVector(X, Y, Z));
		UVs.Add(FVector2D(Y / Radius / 2.0f + 0.5f, X / Radius / 2.0f + 0.5f));
	}

	Normal = FVector::CrossProduct(Vertices[(RingCount+1) * PointsCount + 3], Vertices[(RingCount + 1) * PointsCount + 2]).GetSafeNormal();
	Normals.Add(Normal);

	//Triangles and Normals
	for (int32 i = 1; i <= PointsCount; ++i) {
		if (i < PointsCount) {
			AddTriangle(Triangles, (RingCount + 1)* PointsCount + 1, (RingCount + 1)* PointsCount + (i + 1), ((RingCount + 1) * PointsCount + i + 2));
		}
		else {
			AddTriangle(Triangles, (RingCount + 1)* PointsCount + 1, (RingCount + 1)* PointsCount + (i + 1), ((RingCount + 1)* PointsCount + 2));
		}
		Normals.Add(Normal);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void AVerticalRailActor::GenerateHorizontalHalfCylinder(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, FVector Offset) {

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	//Populating
	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {

			float Phi = PI * PointIndex;
			if (PointsCount > 1) {
				Phi /= (PointsCount - 1);
			}
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			FVector Vertex = FVector{ Radius * CosPhi, Length * (1 - ((float)RingIndex / (RingCount - 1))),  Radius * SinPhi };
			Vertex += Offset;

			Vertices.Add(Vertex);
			UVs.Add(FVector2D{ Phi / (PI),  (1 - ((float)RingIndex / (RingCount - 1))) });

			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				AddTriangle(Triangles, CurrentVertexIndex, NextVertexIndex, NextVertexIndex + 1);
				AddTriangle(Triangles, NextVertexIndex + 1, CurrentVertexIndex + 1, CurrentVertexIndex);
			}
		}
	}

	//Normals
	for (int32 RingIndex = 0; RingIndex < RingCount - 1; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount - 1; ++PointIndex) {
			int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
			int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

			FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
			Normals.Add(Normal);

			if (PointIndex == PointsCount - 2) {
				Normals.Add(Normal);
			}
		}
	}

	for (int32 PointIndex = 0; PointIndex < PointsCount - 1; ++PointIndex) {
		int32 CurrentVertexIndex = (RingCount - 2) * PointsCount + PointIndex;
		int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

		FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[CurrentVertexIndex]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[NextVertexIndex], Vertices[NextVertexIndex + 1])).GetSafeNormal();
		Normals.Add(Normal);

		if (PointIndex == PointsCount - 2) {
			Normals.Add(Normal);
		}
	}

	//Base Circle
	Vertices.Add(Offset);
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
		float Phi = PI * PointIndex;
		if (PointsCount > 1) {
			Phi /= (PointsCount - 1);
		}
		
		FVector Vertex{ Radius * FMath::Cos(Phi), 0, Radius * FMath::Sin(Phi) };
		UVs.Add(FVector2D(Vertex.X / Radius / 2.0f + 0.5f, Vertex.Z / Radius / 2.0f + 0.5f));

		Vertex += Offset;
		Vertices.Add(Vertex);
	}

	FVector Normal = FVector::CrossProduct(Vertices[RingCount * PointsCount + 1], Vertices[RingCount * PointsCount + 2]).GetSafeNormal();
	Normals.Add(Normal);

	//Triangles and Normals
	for (int32 i = 1; i <= PointsCount; ++i) {
		if (i < PointsCount) {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + i + 1));
		}
		else {
			AddTriangle(Triangles, RingCount * PointsCount + i, RingCount * PointsCount, (RingCount * PointsCount + 1));
		}
		Normals.Add(Normal);
	}

	//Top Circle
	Vertices.Add(FVector(Offset.X, Offset.Y + Length, Offset.Z));
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
		float Phi = PI * PointIndex;
		if (PointsCount > 1) {
			Phi /= (PointsCount - 1);
		}

		FVector Vertex{ Radius * FMath::Cos(Phi), Length, Radius * FMath::Sin(Phi) };
		UVs.Add(FVector2D(Vertex.X / Radius / 2.0f + 0.5f, Vertex.Z / Radius / 2.0f + 0.5f));
		
		Vertex += Offset;
		Vertices.Add(Vertex);
	}

	Normal = FVector::CrossProduct(Vertices[(RingCount + 1) * PointsCount + 3], Vertices[(RingCount + 1) * PointsCount + 2]).GetSafeNormal();
	Normals.Add(Normal);

	//Triangles and Normals
	for (int32 i = 1; i <= PointsCount; ++i) {
		if (i < PointsCount) {
			AddTriangle(Triangles, (RingCount + 1)* PointsCount + 1, (RingCount + 1) * PointsCount + (i + 1), ((RingCount + 1) * PointsCount + i + 2));
		}
		else {
			AddTriangle(Triangles, (RingCount + 1)* PointsCount + 1, (RingCount + 1) * PointsCount + (i + 1), ((RingCount + 1) * PointsCount + 2));
		}
		Normals.Add(Normal);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void AVerticalRailActor::GeneratePyramid(int32 Section, const FVector & PyramidDimensions, float ZOffset) {

	//Declarations
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	// Bottom Face
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 0
	Vertices.Add(FVector{ PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 1
	Vertices.Add(FVector{ PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 2
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 3

	// Pyramid Front Side
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 0
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 3
	Vertices.Add(FVector{ 0, 0, ZOffset + PyramidDimensions.Z });

	// Pyramid Right Side
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 3
	Vertices.Add(FVector{ PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 2
	Vertices.Add(FVector{ 0, 0, ZOffset + PyramidDimensions.Z });

	// Pyramid Back Side
	Vertices.Add(FVector{ PyramidDimensions.X / 2, PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 2
	Vertices.Add(FVector{ PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 1
	Vertices.Add(FVector{ 0, 0, ZOffset + PyramidDimensions.Z });

	// Pyramid Left Side
	Vertices.Add(FVector{ PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 1
	Vertices.Add(FVector{ -PyramidDimensions.X / 2, -PyramidDimensions.Y / 2, ZOffset - PyramidDimensions.Z / 2 }); // 0
	Vertices.Add(FVector{ 0, 0, ZOffset + PyramidDimensions.Z });

	// Triangles - Bottom
	AddTriangle(Triangles, 0, 1, 2);
	AddTriangle(Triangles, 2, 3, 0);

	// Triangles - 4 Surface
	for (int32 i = 4; i < 16; i += 3) {
		AddTriangle(Triangles, i, i + 1, i + 2);
	}

	// UVs - Bottom
	UVs.Add(FVector2D{ 0.0, 0.0 });
	UVs.Add(FVector2D{ 0.0, 1.0 });
	UVs.Add(FVector2D{ 1.0, 1.0 });
	UVs.Add(FVector2D{ 1.0, 0.0 });

	// UVs - 4 Surface
	for (int32 i = 4; i < 16; i += 3) {
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
		UVs.Add(FVector2D{ 0.5, 1.0 });
	}

	// Normals - Bottom
	FVector Normal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[0], Vertices[3]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[0], Vertices[1]));
	Normals.Add(Normal);
	Normals.Add(Normal);
	Normals.Add(Normal);
	Normals.Add(Normal);

	// Normals - 4 Surface
	for (int32 i = 4; i < 16; i += 3) {
		FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i]));
		
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void AVerticalRailActor::AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3) {
	Triangles.Add(Index1);
	Triangles.Add(Index2);
	Triangles.Add(Index3);
}
