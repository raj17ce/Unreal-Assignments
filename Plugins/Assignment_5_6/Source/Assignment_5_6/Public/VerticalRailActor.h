// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"

UENUM()
enum class ETopMeshType : uint8 {
	WindsorTurnedCapital,
	RoundTurnedCapital,
	AcornCapital,
	GothicStarTop,
	RoundedOverTop,
	RoundedStarTop,
	PyramidTop
};

UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalRailActor();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Top Mesh")
	ETopMeshType TopMeshType;

	UFUNCTION(BlueprintCallable)
	void CreateVerticalRailActor(const FVector& RailingDimensions);
	void GenerateTopMesh(const FVector& CubeDimensions);

	void GenerateCube(int32 Section, const FVector& CubeDimensions, float ZOffset);
	void GenerateSphere(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset);
	void GenerateOval(int32 Section, float Radius, int32 RingCount, int32 PointsCount, float ZOffset, float ZFactor);
	void GenerateBellShape(int32 Section, float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 SlicesCount, int StacksCount, float ZOffset);
	
	UFUNCTION(BlueprintCallable)
	void GenerateCone(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, float ZOffset);
	UFUNCTION(BlueprintCallable)
	void GenerateCylinder(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, float ZOffset);
	UFUNCTION(BlueprintCallable)
	void GenerateHorizontalHalfCylinder(int32 Section, float Radius, float Length, int32 RingCount, int32 PointsCount, FVector Offset);
	void GeneratePyramid(int32 Section, const FVector& PyramidDimensions, float ZOffset);
	//void GenerateCircle(int32 Section, float Radius, int32 PointsCount, float ZOffset);

	void AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3);
};