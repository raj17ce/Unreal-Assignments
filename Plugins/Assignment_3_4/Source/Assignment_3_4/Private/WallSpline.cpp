// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpline.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWallSpline::AWallSpline() : SourceMesh{nullptr}, SourceMaterial{nullptr}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(RootScene);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(RootScene);

	SplineComponent->ClearSplinePoints();
}

// Called when the game starts or when spawned
void AWallSpline::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWallSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallSpline::OnConstruction(const FTransform& Transform) {
	
}

void AWallSpline::GenerateSplineMeshes() {

	int32 SplinePointIndex = SplineComponent->GetNumberOfSplinePoints() - 2;

	if (SplinePointIndex < 0) {
		return;
	}

	SetSplinePointZeroTangents(SplinePointIndex);

	FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(SplinePointIndex, ESplineCoordinateSpace::World);
	FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(SplinePointIndex + 1, ESplineCoordinateSpace::World);

	CreateSplineMeshComponent(StartLocation, EndLocation);
}

void AWallSpline::SetSplinePointZeroTangents(int32 index) {
	FVector ZeroVector{ 0,0,0 };

	SplineComponent->SetTangentsAtSplinePoint(index, ZeroVector, ZeroVector, ESplineCoordinateSpace::World);
	SplineComponent->SetTangentsAtSplinePoint(index + 1, ZeroVector, ZeroVector, ESplineCoordinateSpace::World);
}

void AWallSpline::CreateSplineMeshComponent(FVector& StartLocation, FVector& EndLocation) {
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(SplineComponent, USplineMeshComponent::StaticClass());

	if (SourceMesh) {
		if (SourceMaterial) {
			SourceMesh->SetMaterial(0, SourceMaterial);
		}
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetStaticMesh(SourceMesh);
	}

	FVector Tangent = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, EndLocation) * 10;

	StartLocation -= Tangent;
	EndLocation += Tangent;

	SplineMeshComponent->SetStartAndEnd(StartLocation, Tangent, EndLocation, Tangent);
	SplineMeshComponent->RegisterComponent();
	SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SplineMeshComponents.Add(SplineMeshComponent);
}

void AWallSpline::AddNewSplinePoint(FVector& Location) {
	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	GenerateSplineMeshes();
}

void AWallSpline::DestroySplineMeshes() {
	for (int32 i = 0; i < SplineMeshComponents.Num(); ++i) {
		if (SplineMeshComponents[i]) {
			SplineMeshComponents[i]->DestroyComponent();
			SplineMeshComponents[i] = nullptr;
		}
	}

	if (!SplineMeshComponents.IsEmpty()) {
		SplineMeshComponents.Empty();
	}
}

bool AWallSpline::RemoveLastSplinePoint() {
	int32 LastIndex = SplineMeshComponents.Num() - 1;
	bool IsSplineDeleted{ false };
	
	if (LastIndex >= 0) {
		SplineComponent->RemoveSplinePoint(LastIndex+1);

		if (SplineMeshComponents[LastIndex]) {
			SplineMeshComponents[LastIndex]->DestroyComponent();
			SplineMeshComponents[LastIndex] = nullptr;
		}

		SplineMeshComponents.RemoveAt(LastIndex);

		if (LastIndex == 0) {
			SplineComponent->ClearSplinePoints();
			IsSplineDeleted = true;
		}
	}
	return IsSplineDeleted;
}