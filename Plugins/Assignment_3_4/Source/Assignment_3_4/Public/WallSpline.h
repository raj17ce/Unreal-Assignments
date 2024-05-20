// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "WallSpline.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API AWallSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Defaults")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Spline")
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	UMaterial* SourceMaterial;

	UFUNCTION(BlueprintCallable)
	void GenerateSplineMeshes();

	void SetSplinePointZeroTangents(int32 index);
	void CreateSplineMeshComponent(FVector& StartLocation, FVector& EndLocation);
	void AddNewSplinePoint(FVector& Location);
	void DestroySplineMeshes();
	bool RemoveLastSplinePoint();
};